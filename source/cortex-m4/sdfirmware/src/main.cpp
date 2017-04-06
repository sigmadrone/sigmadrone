/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */

// ----------------------------------------------------------------------------


#include <stdint.h>
#include <stdio.h>
#include <stdexcept>

#include "digitalin.h"
#include "digitalout.h"
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "spimaster.h"
#include "spislave.h"
#include "uart.h"
#include "l3gd20.h"
#include "lsm303d.h"
#include "lps25hb.h"
#include "d3math.h"
#include "hwtimer.h"
#include "pwmencoder.h"
#include "pwmdecoder.h"
#include "timestamp.h"
#include "colibripwm.h"
#include "colibritrace.h"
#include "flightcontrol.h"
#include "uartrpcserver.h"
#include "librexjson/rexjson++.h"
#include "libattitude/attitudetracker.h"
#include "bmp280reader.h"
#include "sensorsprefilters.h"
#include "flashmemory.h"
#include "adc.h"
#include "battery.h"
#include "gpsreader.h"
#include "iirfilt.h"
#include "l3gd20reader.h"
#include "lsm303reader.h"
#include "usbstoragedevice.h"
#include "poweroff.h"

__attribute__((__section__(".user_data"))) uint8_t flashregion[1024];
void* __dso_handle = 0;

DigitalOut ledusb(USB_OTG_LED_PIN);
DigitalOut led1(USER_LED1_PIN);
DigitalOut led2(USER_LED2_PIN);
DigitalOut led3(USER_LED3_PIN);
DigitalOut led4(USER_LED4_PIN);
DigitalOut led5(USER_LED5_PIN);
DigitalOut sesnsor_ctrl(SENSOR_CTRL_PIN, DigitalOut::OutputDefault, DigitalOut::PullDefault, 0);

DigitalIn user_sw1(USER_SWITCH_1_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw2(USER_SWITCH_2_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw3(USER_SWITCH_3_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw4(USER_SWITCH_4_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);

PowerOff poweroff_button(PH_10, PI_6, PI_7, PH_11, {USER_LED1_PIN, USER_LED2_PIN, USER_LED3_PIN});

TaskHandle_t main_task_handle = 0;
TaskHandle_t bmp280_task_handle = 0;
TaskHandle_t battery_task_handle = 0;
TaskHandle_t gps_task_handle = 0;
TaskHandle_t uart_task_handle = 0;
DroneState* drone_state = 0;

FlashMemory configdata(&flashregion, sizeof(flashregion), FLASH_SECTOR_23, 1);

UART uart2({
	{USART2_CTS_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART2},
	{USART2_RTS_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART2},
	{USART2_TX_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART2},
	{USART2_RX_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART2},
},
		USART2,
		DMA1,
		DMA1_Stream6,		/* TX DMA stream */
		DMA_CHANNEL_4,		/* TX DMA channel */
		DMA1_Stream5,		/* RX DMA stream */
		DMA_CHANNEL_4,		/* RX DMA channel */
		UART_HWCONTROL_NONE,/* UART_HWCONTROL_RTS_CTS, UART_HWCONTROL_RTS, UART_HWCONTROL_CTS, UART_HWCONTROL_NONE */
		460800,
		250
);

void dronestate_boot_config(DroneState& state)
{
	rexjson::value bootconfig;

	try {
		bootconfig.read((char*)configdata.mem_, configdata.size());
		state.init_from_boot_config(bootconfig);
	} catch (std::exception& e) {}
}

void bmp280_task(void *pvParameters)
{
	(void)pvParameters;

	vTaskDelay(600 / portTICK_RATE_MS);

	I2CMaster i2c(I2C1, 100000, I2C_DUTYCYCLE_2, I2C_ADDRESSINGMODE_7BIT, 750, {
				{I2C1_SCL_PIN, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF4_I2C1},
				{I2C1_SDA_PIN, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF4_I2C1},
		});

again:
	try {
		TimeStamp led_toggle_ts;
		PerfCounter loop_time;
		BMP280 bmp2(i2c, 0xee);
		bmp2.set_oversamp_pressure(BMP280_OVERSAMP_16X);
		bmp2.set_work_mode(BMP280_ULTRA_HIGH_RESOLUTION_MODE);
		bmp2.set_filter(BMP280_FILTER_COEFF_16);

		Bmp280Reader* bmp_reader = new Bmp280Reader(bmp2);
		bmp_reader->calibrate();
		while (1) {
			loop_time.begin_measure();
			if (led_toggle_ts.elapsed() > TimeSpan::from_seconds(1)) {
				led_toggle_ts.time_stamp();
				led1.toggle();
			}

			bmp_reader->pressure_filter_.set_alpha(drone_state->altitude_lpf_);
			drone_state->altitude_ = bmp_reader->get_altitude(true);
			drone_state->pressure_hpa_ = bmp_reader->get_pressure().hpa();
			drone_state->temperature_ = bmp_reader->get_temperature(false).celsius();
			vTaskDelay(15 / portTICK_RATE_MS);
			loop_time.end_measure();
		}
	} catch (std::exception& e) {
		std::cout << "bmp280_task exception: " << e.what() << std::endl;
		i2c.reinit();
	}
	goto again;
}

void gps_task(void *pvParameters)
{
	(void)pvParameters;
	GPSReader gps;
	PerfCounter gps_measure_time;

	gps.start();
	while (!gps.update_state()) {
		vTaskDelay(200 / portTICK_RATE_MS);
	}
	gps_measure_time.begin_measure();
	while (1) {
		if (gps.update_state()) {
			gps_measure_time.end_measure();
			gps_measure_time.begin_measure();
			drone_state->latitude_ = gps.lattitude();
			drone_state->longitude_ = gps.longitude();
			drone_state->gps_altitude_ = gps.altitude();
			drone_state->speed_over_ground_ = gps.speed();
			drone_state->course_ = gps.course();
			drone_state->satellite_count_ = gps.satellite_count();
		} else {
			vTaskDelay(20 / portTICK_RATE_MS);
		}
	}
}


void battery_task(void *pvParameters)
{
	(void)pvParameters;

	colibri::Voltmeter voltmeter;
	Battery battery;
	while (1) {
		battery.update(voltmeter.measure());
		drone_state->battery_voltage_ = battery.voltage();
		drone_state->battery_percentage_ = battery.charge_percentage();
		drone_state->battery_type_ = battery.type();
		vTaskDelay(3000 / portTICK_RATE_MS);
	}
}

void uart_task(void *pvParameters)
{
	char buffer[128];
	char *p;
	uint8_t tmp;
	(void)pvParameters;

	// Enable nCTS
	uart2.handle_.Instance->CR3 |= (USART_CR3_CTSE);
	for (unsigned int i = 0; true; i++) {
		snprintf(buffer, sizeof(buffer), "{\"uart_task\": %d}\n", i);
		for (p = buffer; *p; p++) {
			uart2.rx(&tmp, 0);
			if (uart2.tx(*p, 250) != HAL_OK)
				break;
		}
		vTaskDelay(350 / portTICK_RATE_MS);
	}
}

void main_task(void *pvParameters)
{
	(void) pvParameters;

	vTaskDelay(500 / portTICK_RATE_MS);

	SPIMaster spi5(SPI5, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{MEMS_SPI_SCK_PIN,  GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},
				{MEMS_SPI_MISO_PIN, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},
				{MEMS_SPI_MOSI_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL,   GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},
			}, {
				{GYRO_CS_PIN,  GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},
				{ACCEL_CS_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},
			});

	SPIMaster spi2(SPI2, SPI_BAUDRATEPRESCALER_16, 0x2000, {
					{EXT_MEMS_SPI_SCK_PIN,  GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI2},
					{EXT_MEMS_SPI_MISO_PIN, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI2},
					{EXT_MEMS_SPI_MOSI_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL,   GPIO_SPEED_MEDIUM, GPIO_AF5_SPI2},
				}, {
					{EXT_GYRO_CS_PIN,  GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},
					{EXT_PRES_CS_PIN,  GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},
				});

	L3GD20 gyro(spi5, 0);
	LPS25HB lps25hb(spi2, 1);
	LSM303D accel(spi5, 1);
	uint8_t gyro_wtm = 5;
	uint8_t acc_wtm = 8;
	TimeStamp console_update_time;
	TimeStamp sample_dt;
	TimeStamp led_toggle_ts;
	FlightControl flight_ctl;
	static bool print_to_console = false;
	LowPassFilter<Vector3f, float> gyro_lpf({0.5});
	LowPassFilter<Vector3f, float> acc_lpf_alt({0.9});
	LowPassFilter<Vector3f, float> acc_lpf_att({0.990});
	LowPassFilter<float, float> pressure_lpf({0.6});
	attitudetracker att;

	/*
	 * Apply the boot configuration from flash memory.
	 */
	dronestate_boot_config(*drone_state);

#if defined USE_ALIGNMENT_MIPIFRONT

	static const Matrix3f gyro_align(
            0,-1, 0,
            1, 0, 0,
            0, 0, 1);

	static const Matrix3f acc_align(
            0, -1, 0,
            1, 0, 0,
            0, 0, 1);

#elif defined USE_ALIGNMENT_MIPIFRONT_2F
	static const Matrix3f gyro_align(
            0,-1, 0,
            1, 0, 0,
            0, 0, 1);

	static const Matrix3f acc_align(
            0, 1, 0,
           -1, 0, 0,
            0, 0, 1);


#elif USE_ALIGNMENT_2F
	static const Matrix3f gyro_align(
	        1, 0, 0,
            0, 1, 0,
		    0, 0, 1);

	static const Matrix3f acc_align(
	        -1, 0, 0,
            0, -1, 0,
	        0,  0, 1);
#else
	static const Matrix3f gyro_align(
	        1, 0, 0,
            0, 1, 0,
		    0, 0, 1);

	static const Matrix3f acc_align(
	        1, 0, 0,
            0, 1, 0,
	        0, 0, 1);
#endif


	L3GD20Reader gyro_reader(gyro, GYRO_INT2_PIN, gyro_align);
	LSM303Reader acc_reader(accel, ACC_INT2_PIN, acc_align);

	UartRpcServer rpcserver(*drone_state, configdata, acc_reader.mag_calibrator_);

	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA1_Stream6_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA1_Stream5_IRQn);

#ifndef ENABLE_UART_TASK
	uart2.uart_dmarx_start();
#endif

	printf("Priority Group: %lu\n", NVIC_GetPriorityGrouping());
	printf("SysTick_IRQn priority: %lu\n", NVIC_GetPriority(SysTick_IRQn) << __NVIC_PRIO_BITS);
	printf("configKERNEL_INTERRUPT_PRIORITY: %d\n", configKERNEL_INTERRUPT_PRIORITY);
	printf("configMAX_SYSCALL_INTERRUPT_PRIORITY: %d\n", configMAX_SYSCALL_INTERRUPT_PRIORITY);
	printf("LPS25HB Device id: %d\n", lps25hb.Get_DeviceID());
	vTaskDelay(500 / portTICK_RATE_MS);

	UsbStorageDevice::start();

	gyro_reader.init(gyro_wtm);
	gyro_reader.enable_int2(false);

	vTaskDelay(500 / portTICK_RATE_MS);

	acc_reader.init(acc_wtm);
	acc_reader.enable_int2(false);
	vTaskDelay(500 / portTICK_RATE_MS);

	printf("Calibrating...");

	gyro_reader.enable_int2(true);
	gyro_reader.calculate_static_bias_filtered(2400);
	printf(" Done!\n");
	flight_ctl.start_receiver();

	printf("Entering main loop...\n");
	gyro_reader.enable_int2(true);

	sample_dt.time_stamp();
	lps25hb.Set_FifoMode(LPS25HB_FIFO_STREAM_MODE);
	lps25hb.Set_FifoModeUse(LPS25HB_ENABLE);
	lps25hb.Set_Odr(LPS25HB_ODR_25HZ);
	lps25hb.Set_Bdu(LPS25HB_BDU_NO_UPDATE);
	LPS25HB_FIFOTypeDef_st fifo_config;
	memset(&fifo_config, 0, sizeof(fifo_config));
	lps25hb.Get_FifoConfig(&fifo_config);

#ifdef USE_LPS25HB
	float base_pressure = lps25hb.Get_PressureHpa();
	for (int i = 0; i < 100; i++) {
		while (lps25hb.Get_FifoStatus().FIFO_EMPTY)
			vTaskDelay(50 / portTICK_RATE_MS);
		base_pressure = pressure_lpf.do_filter(lps25hb.Get_PressureHpa());
	}
#endif

	// Infinite loop
	while (1) {
		drone_state->iteration_++;
#ifdef USE_LPS25HB
		if (drone_state->iteration_ % 120 == 0)
			led1.toggle();
		if (drone_state->iteration_ % 5 == 0) {
			drone_state->temperature_ = lps25hb.Get_TemperatureCelsius();
			while (!lps25hb.Get_FifoStatus().FIFO_EMPTY) {
				drone_state->pressure_hpa_ = pressure_lpf.do_filter(lps25hb.Get_PressureHpa());
				float alt = (powf(base_pressure/drone_state->pressure_hpa_, 0.1902f) - 1.0f) * ((lps25hb.Get_TemperatureCelsius()) + 273.15f)/0.0065;
				drone_state->altitude_ = Distance::from_meters(alt);
			}
		}
#endif

		gyro_reader.wait_for_data();
		drone_state->dt_ = sample_dt.elapsed();
		sample_dt.time_stamp();

		if (drone_state->base_throttle_ > 0.1)
			att.accelerometer_correction_speed(drone_state->accelerometer_correction_speed_);
		else
			att.accelerometer_correction_speed(3.0f);
		att.gyro_drift_pid(drone_state->gyro_drift_kp_, drone_state->gyro_drift_ki_, drone_state->gyro_drift_kd_);
		att.gyro_drift_leak_rate(drone_state->gyro_drift_leak_rate_);

		size_t fifosize = gyro_reader.size();
		for (size_t i = 0; i < fifosize; i++)
			   drone_state->gyro_raw_ = gyro_lpf.do_filter(gyro_reader.read_sample());
		if (drone_state->gyro_raw_.length_squared() > 0 && drone_state->dt_.microseconds() > 0) {
			   drone_state->gyro_ = (drone_state->gyro_raw_ - gyro_reader.bias()) * drone_state->gyro_factor_;
			   att.track_gyroscope(DEG2RAD(drone_state->gyro_) * 1.0f, drone_state->dt_.seconds_float());
		}

		fifosize = acc_reader.size();
		for (size_t i = 0; i < fifosize; i++) {
			Vector3f acc_sample = acc_reader.read_sample_acc();
			acc_lpf_att.do_filter(acc_sample);
			acc_lpf_alt.do_filter(acc_sample);
		}
		drone_state->accel_raw_ = acc_lpf_att.output();
		drone_state->accel_alt_ = acc_lpf_alt.output();
		drone_state->accel_ = (drone_state->accel_raw_ - drone_state->accelerometer_adjustment_).normalize();

#define ALLOW_ACCELEROMETER_OFF
#ifdef ALLOW_ACCELEROMETER_OFF
		if (drone_state->track_accelerometer_) {
			att.track_accelerometer(drone_state->accel_, drone_state->dt_.seconds_float());
		}
#else
		att.track_accelerometer(drone_state->accel_, drone_state->dt_.seconds_float());
#endif

#define REALTIME_DATA 0
#if REALTIME_DATA
		std::cout << drone_state->gyro_.transpose() << drone_state->accel_.transpose() << drone_state->pid_torque_.transpose();
		std::cout << drone_state->dt_.seconds_float() << std::endl;
#endif

		drone_state->mag_raw_ = acc_reader.read_sample_mag();
		drone_state->mag_ = drone_state->mag_raw_.normalize();
		if (drone_state->track_magnetometer_) {
			att.track_magnetometer(drone_state->mag_, drone_state->dt_.seconds_float());
		}

		drone_state->attitude_ = att.get_attitude();
		drone_state->gyro_drift_error_ = RAD2DEG(att.get_drift_error());

		flight_ctl.update_state(*drone_state);
		flight_ctl.send_throttle_to_motors();

		if (print_to_console && console_update_time.elapsed() > TimeSpan::from_milliseconds(300)) {

			Vector3f drift_err = att.get_drift_error();
			console_update_time.time_stamp();
			printf("Gyro      : %5.3f %5.3f %5.3f\n", drone_state->gyro_.at(0), drone_state->gyro_.at(1), drone_state->gyro_.at(2));
			printf("Drift Err : %5.3f %5.3f %5.3f\n", RAD2DEG(drift_err.at(0)), RAD2DEG(drift_err.at(1)), RAD2DEG(drift_err.at(2)));
			printf("Gyro Raw  : %5.3f %5.3f %5.3f\n", drone_state->gyro_raw_.at(0), drone_state->gyro_raw_.at(1), drone_state->gyro_raw_.at(2));
			printf("Accel     : %5.3f %5.3f %5.3f\n", drone_state->accel_.at(0), drone_state->accel_.at(1), drone_state->accel_.at(2));
			printf("Mag       : %5.3f %5.3f %5.3f\n", drone_state->mag_.at(0), drone_state->mag_.at(1), drone_state->mag_.at(2));
			printf("dT        : %lu uSec\n", (uint32_t)drone_state->dt_.microseconds());
			printf("Q         : %5.3f %5.3f %5.3f %5.3f\n\n", drone_state->attitude_.w, drone_state->attitude_.x, drone_state->attitude_.y,
					drone_state->attitude_.z);
#if 1
			printf("Motors    : %1.2f %1.2f %1.2f %1.2f\n", drone_state->motors_[0], drone_state->motors_[1],
								drone_state->motors_[2], drone_state->motors_[3]);
			printf("Throttle  : %1.2f\n", drone_state->base_throttle_);
		    printf("Armed     : %d\n", drone_state->motors_armed_);
			printf("Altitude  : %4.2f m\n", drone_state->altitude_.meters());
			printf("GPS       : Lon: %3.4f Lat: %3.4f Sat %lu Alt: %4.2f m\n",
					drone_state->longitude_.degrees(), drone_state->latitude_.degrees(),
					drone_state->satellite_count_, drone_state->gps_altitude_.meters());
			printf("Battery   : %2.1f V, %2.0f%%\n", drone_state->battery_voltage_.volts(), drone_state->battery_percentage_);
#endif
		}

#if 0
		if (led_toggle_ts.elapsed() > TimeSpan::from_seconds(1)) {
			led_toggle_ts.time_stamp();
			led0.toggle();
		}
#endif

#ifndef ENABLE_UART_TASK
		rpcserver.jsonrpc_request_handler(&uart2);
#endif
	}
}

extern void relocate_interrupt_table();

int main(int argc, char* argv[])
{
	uint32_t freq = HAL_RCC_GetSysClockFreq();
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();

	(void) argc;
	(void) argv;

	relocate_interrupt_table();

	TimeStamp::init();
	colibri::UartTrace::init(115200 * 2);
	drone_state = new DroneState();

	printf("Starting main_task:, CPU freq: %lu, PCLK1 freq: %lu, PCLK2 freq: %lu\n", freq, pclk1, pclk2);

	/* Create tasks */
	xTaskCreate(
			main_task, /* Function pointer */
			"main_task", /* Task name - for debugging only*/
			4 * configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 3UL, /* Task priority*/
			&main_task_handle /* Task handle */
	);

#ifndef USE_LPS25HB
	xTaskCreate(
			bmp280_task, /* Function pointer */
			"bmp280_task", /* Task name - for debugging only*/
			configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 2UL, /* Task priority*/
			&bmp280_task_handle /* Task handle */
	);
#endif

	xTaskCreate(
			battery_task, /* Function pointer */
			"battery_task", /* Task name - for debugging only*/
			configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 1UL, /* Task priority*/
			&battery_task_handle /* Task handle */
	);

	xTaskCreate(
			gps_task, /* Function pointer */
			"gps_task", /* Task name - for debugging only*/
			configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 1UL, /* Task priority*/
			&gps_task_handle /* Task handle */
	);


#if ENABLE_UART_TASK
	xTaskCreate(
			uart_task, /* Function pointer */
			"uart_task", /* Task name - for debugging only*/
			configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 1UL, /* Task priority*/
			&uart_task_handle /* Task handle */
	);
#endif

	/*
	 * Disable the SysTick_IRQn and clean the priority
	 * and let the scheduler configure and enable it.
	 */
	NVIC_DisableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 0);

	vTaskStartScheduler(); // this call will never return
}

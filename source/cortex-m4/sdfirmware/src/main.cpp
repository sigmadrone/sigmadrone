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
#include "bmp180reader.h"
#include "sensorsprefilters.h"
#include "flashmemory.h"
#include "adc.h"
#include "battery.h"
#include "gpsreader.h"
#include "iirfilt.h"
#include "l3gd20reader.h"
#include "usbstoragedevice.h"

__attribute__((__section__(".user_data"))) uint8_t flashregion[1024];
void* __dso_handle = 0;

DigitalOut ledusb(USB_OTG_LED_PIN);
DigitalOut led1(USER_LED1_PIN);
DigitalOut led2(USER_LED2_PIN);
DigitalOut led3(USER_LED3_PIN);
DigitalOut led4(USER_LED4_PIN);
DigitalOut led5(USER_LED5_PIN);

DigitalOut sesnsor_ctrl(SENSOR_CTRL_PIN, DigitalOut::OutputDefault, DigitalOut::PullDefault, 0);
DigitalOut pwr_on(PWR_ON_PIN, DigitalOut::OutputDefault, DigitalOut::PullDefault, 1);

DigitalIn user_sw1(USER_SWITCH_1_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw2(USER_SWITCH_2_PIN, DigitalIn::PullNone, DigitalIn::InterruptFalling);
DigitalIn user_sw3(USER_SWITCH_3_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw4(USER_SWITCH_4_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw5(USER_SWITCH_5_PIN, DigitalIn::PullNone, DigitalIn::InterruptDefault);

TaskHandle_t main_task_handle = 0;
TaskHandle_t bmp180_task_handle = 0;
TaskHandle_t battery_task_handle = 0;
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

void bmp180_task(void *pvParameters)
{
	(void)pvParameters;

	vTaskDelay(600 / portTICK_RATE_MS);

	I2CMaster i2c(I2C1, 100000, I2C_DUTYCYCLE_2, I2C_ADDRESSINGMODE_7BIT, 750, {
				{I2C1_SCL_PIN, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF4_I2C1},
				{I2C1_SDA_PIN, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF4_I2C1},
		});
	BMP180 bmp(i2c);
	TimeStamp led_toggle_ts;
	GPSReader gps;

	Bmp180Reader* bmp_reader = new Bmp180Reader(bmp);
	PerfCounter gps_measure_time;
	PerfCounter loop_time;

	bmp_reader->calibrate();

	gps.start();
	while (1) {
		loop_time.begin_measure();
		if (led_toggle_ts.elapsed() > TimeSpan::from_seconds(1)) {
			led_toggle_ts.time_stamp();
			led1.toggle();
		}

		try {
			bmp_reader->pressure_filter_.set_alpha(drone_state->altitude_lpf_);
			drone_state->altitude_ = bmp_reader->altitude_meters(true);
			drone_state->pressure_hpa_ = bmp_reader->pressure_hpa();
			drone_state->temperature_ = bmp_reader->temperature_celsius(false);
			gps_measure_time.begin_measure();
			gps.update_state();
			gps_measure_time.end_measure();
			drone_state->latitude_ = gps.lattitude();
			drone_state->longitude_ = gps.longitude();
			drone_state->gps_altitude_ = gps.altitude();
			drone_state->speed_over_ground_ = gps.speed();
			drone_state->course_ = gps.course();
			drone_state->satellite_count_ = gps.satellite_count();
		} catch (std::exception& e) {
//			i2c.reinit();
		}
		loop_time.end_measure();

		// Do not delay the thread here, it will be delayed when the sensor is being read
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

static Vector3f read_accelerometer(
		LSM303D& acc,
		LowPassFilter<Vector3f, float>& lpf_filt)
{
	Vector3f acc_filtered;
	uint8_t count = acc.GetFifoSourceFSS();
	if (count > 0) {
		for (uint8_t i = 0; i < count; i++) {
			LSM303D::AxesAcc_t axes = {0,0,0};
			acc.GetAcc(&axes);
			acc_filtered = lpf_filt.do_filter(Vector3d(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z));
		}
	}
	return acc_filtered;
}

static void warmup_accelerometer_filter(
		LSM303D& acc,
		LowPassFilter<Vector3f, float>& lpf_filt)
{
	static const size_t numSamples = 256;
	for (size_t i = 0; i < numSamples;) {
		if (read_accelerometer(acc, lpf_filt).length() > 0) {
			++i;
		}
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
				});

	L3GD20 gyro(spi5, 0);
	L3GD20 ext_gyro(spi2, 0);
	LSM303D accel(spi5, 1);
	uint8_t gyro_wtm = 5;
	uint8_t acc_wtm = 8;
	LSM303D::AxesMag_t mag_axes;
	attitudetracker att;
	TimeStamp console_update_time;
	TimeStamp sample_dt;
	TimeStamp led_toggle_ts;
	FlightControl flight_ctl;
	UartRpcServer rpcserver(*drone_state, configdata);
	MagLowPassPreFilter3d* mag_lpf = new MagLowPassPreFilter3d();
	bool ext_gyro_present = false;
	static bool print_to_console = false;
	LowPassFilter<Vector3f, float> gyro_lpf({0.5, 0.5});
	LowPassFilter<Vector3f, float> acc_lpf({0.90, 0.1});

	/*
	 * Apply the boot configuration from flash memory.
	 */
	dronestate_boot_config(*drone_state);

#ifdef USE_SIXPROPELLERS

	static const Matrix3f gyro_align(
	        0,-1, 0,
            1, 0, 0,
		    0, 0, 1);

	static const Matrix3f acc_align(
	        0, 1, 0,
           -1, 0, 0,
	        0, 0, 1);

#else
	static const Matrix3f gyro_align(
	        0,-1, 0,
           -1, 0, 0,
		    0, 0,-1);

	static const Matrix3f acc_align(
	        0,-1, 0,
           -1, 0, 0,
	        0, 0,-1);
#endif

	L3GD20Reader gyro_reader(gyro, GYRO_INT2_PIN, gyro_align);
	L3GD20Reader ext_gyro_reader(ext_gyro, EXT_GYRO_INT2_PIN, drone_state->external_gyro_align_);

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
	vTaskDelay(500 / portTICK_RATE_MS);

	UsbStorageDevice::start();

	gyro_reader.init_gyro(gyro_wtm);
	gyro_reader.enable_disable_int2(false);

	vTaskDelay(500 / portTICK_RATE_MS);

	try {
		ext_gyro_reader.init_gyro(gyro_wtm);
		ext_gyro_reader.enable_disable_int2(false);
		ext_gyro_present = true;
	} catch(...) {
		ext_gyro_present = false;
	}

	accel.SetHPFMode(LSM303D::HPM_NORMAL_MODE_RES);
	accel.SetFilterDataSel(LSM303D::MEMS_DISABLE);
	accel.SetODR(LSM303D::ODR_1600Hz);
	accel.SetFullScale(LSM303D::FULLSCALE_8);
	accel.SetAxis(LSM303D::X_ENABLE | LSM303D::Y_ENABLE | LSM303D::Z_ENABLE);
	accel.FIFOModeSet(LSM303D::FIFO_STREAM_MODE);
	accel.SetThreshold(acc_wtm);
	accel.SetInt2Pin(LSM303D_INT2_OVERRUN_ENABLE|LSM303D_INT2_FTH_ENABLE);
	accel.SetInt2Pin(0);
	accel.SetODR_M(LSM303D::ODR_100Hz_M);
	accel.SetFullScaleMag(LSM303D::FULLSCALE_2_GA);
	accel.SetModeMag(LSM303D::CONTINUOUS_MODE);

	vTaskDelay(500 / portTICK_RATE_MS);

	printf("Calibrating...");

	if (ext_gyro_present) {
		ext_gyro_reader.enable_disable_int2(true);
		ext_gyro_reader.calculate_static_bias(400);
		ext_gyro_reader.enable_disable_int2(false);
		if (ext_gyro_reader.bias().length() == 0) {
			ext_gyro_present = false;
		}
	}

	gyro_reader.enable_disable_int2(true);
	gyro_reader.calculate_static_bias_filtered(1200);

	warmup_accelerometer_filter(accel, acc_lpf);

	printf(" Done!\n");
	flight_ctl.start_receiver();

	printf("Entering main loop...\n");
	L3GD20Reader* active_gyro = &gyro_reader;
	active_gyro->enable_disable_int2(true);

	sample_dt.time_stamp();

	// Infinite loop
	while (1) {
		drone_state->iteration_++;

		L3GD20Reader* desired_gyro_reader = (drone_state->external_gyro_enabled_ && ext_gyro_present)  ? &ext_gyro_reader : &gyro_reader;
		if (active_gyro != desired_gyro_reader) {
			active_gyro->read_data(gyro_wtm);
			active_gyro->enable_disable_int2(false);
			active_gyro = desired_gyro_reader;
			active_gyro->enable_disable_int2(true);
		}

		active_gyro->wait_for_data();

		drone_state->dt_ = sample_dt.elapsed();
		sample_dt.time_stamp();

		if (drone_state->base_throttle_ > 0.1)
			att.accelerometer_correction_speed(drone_state->accelerometer_correction_speed_);
		else
			att.accelerometer_correction_speed(3.0f);
		att.gyro_drift_pid(drone_state->gyro_drift_kp_, drone_state->gyro_drift_ki_, drone_state->gyro_drift_kd_);
		att.gyro_drift_leak_rate(drone_state->gyro_drift_leak_rate_);

		size_t fifosize = active_gyro->size();
		for (size_t i = 0; i < fifosize; i++)
			drone_state->gyro_raw_ = gyro_lpf.do_filter(active_gyro->read_sample());
		if (drone_state->gyro_raw_.length_squared() > 0 && drone_state->dt_.microseconds() > 0) {
			drone_state->gyro_ = (drone_state->gyro_raw_ - active_gyro->bias()) * drone_state->gyro_factor_;
			att.track_gyroscope(DEG2RAD(drone_state->gyro_), drone_state->dt_.seconds_float());
		}

		drone_state->accel_raw_ = acc_align * read_accelerometer(accel, acc_lpf);
		if (drone_state->accel_raw_.length_squared() > 0) {
			Vector3f accel_adjusted = drone_state->accel_raw_ - drone_state->accelerometer_adjustment_;
			drone_state->accel_ = accel_adjusted.normalize();
		}

#define ALLOW_ACCELEROMETER_OFF
#define REALTIME_DATA 0
#if REALTIME_DATA
		std::cout << drone_state->gyro_.x() << "," << drone_state->gyro_.y() << "," << drone_state->gyro_.z() << ",";
		std::cout << drone_state->accel_.x() << "," << drone_state->accel_.y() << "," << drone_state->accel_.z() << "," ;
		std::cout << drone_state->pid_torque_.x() << "," << drone_state->pid_torque_.y() << "," << drone_state->pid_torque_.z() << ",";
		std::cout << drone_state->dt_.seconds_float() << std::endl;
#endif

#ifdef ALLOW_ACCELEROMETER_OFF
		if (drone_state->track_accelerometer_)
			att.track_accelerometer(drone_state->accel_, drone_state->dt_.seconds_float());
#else
		att.track_accelerometer(drone_state->accel_, drone_state->dt_.seconds_float());
#endif

		accel.GetMag(&mag_axes);
		drone_state->mag_raw_ = acc_align * Vector3f(mag_axes.AXIS_X, mag_axes.AXIS_Y, mag_axes.AXIS_Z);
		drone_state->mag_ = mag_lpf->do_filter(drone_state->mag_raw_.normalize());
		if (drone_state->track_magnetometer_)
			att.track_magnetometer(drone_state->mag_, drone_state->dt_.seconds_float());

		drone_state->attitude_ = att.get_attitude();
		drone_state->gyro_drift_error_ = RAD2DEG(att.get_drift_error());

		flight_ctl.update_state(*drone_state);
		flight_ctl.send_throttle_to_motors();
		QuaternionF attitude_twist, attitude_swing;
		QuaternionF::decomposeTwistSwing(drone_state->attitude_, Vector3f(0,0,1), attitude_swing, attitude_twist);

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
					drone_state->longitude_, drone_state->latitude_,
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

	xTaskCreate(
			bmp180_task, /* Function pointer */
			"bmp180_task", /* Task name - for debugging only*/
			configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 2UL, /* Task priority*/
			&bmp180_task_handle /* Task handle */
	);

	xTaskCreate(
			battery_task, /* Function pointer */
			"battery_task", /* Task name - for debugging only*/
			configMINIMAL_STACK_SIZE, /* Stack depth in words */
			(void*) NULL, /* Pointer to tasks arguments (parameter) */
			tskIDLE_PRIORITY + 1UL, /* Task priority*/
			&battery_task_handle /* Task handle */
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

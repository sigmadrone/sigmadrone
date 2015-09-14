//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

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
#include "matrix.h"
#include "hwtimer.h"
#include "pwmencoder.h"
#include "pwmdecoder.h"
#include "timestamp.h"
#include "tm_stm32f4_ili9341.h"
#include "colibripwm.h"
#include "colibritrace.h"
#include "flightcontrol.h"
#include "uartrpcserver.h"
#include "librexjson/rexjson++.h"
#include "libattitude/attitudetracker.h"
#include "bmp180reader.h"
#include "accellowpassfilter.h"
#include "flashmemory.h"
#include "adc.h"

__attribute__((__section__(".user_data"))) uint8_t flashregion[1024];
void* __dso_handle = 0;

DigitalOut ledusb(PC_4);
DigitalOut gpspwr(PB_0, DigitalOut::OutputDefault, DigitalOut::PullDefault, 1);
DigitalIn gyro_int2(PA_2, DigitalIn::PullNone, DigitalIn::InterruptRising);
DigitalIn user_sw5(PG_2, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw1(PG_3, DigitalIn::PullNone, DigitalIn::InterruptFalling);
DigitalIn user_sw2(PG_6, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw3(PG_7, DigitalIn::PullNone, DigitalIn::InterruptDefault);
DigitalIn user_sw4(PG_11, DigitalIn::PullNone, DigitalIn::InterruptDefault);

TaskHandle_t main_task_handle = 0;
TaskHandle_t bmp180_task_handle = 0;
TaskHandle_t battery_task_handle = 0;
QueueHandle_t hGyroQueue;
TimeStamp isr_ts;
DroneState* drone_state = 0;

FlashMemory configdata(&flashregion, sizeof(flashregion), FLASH_SECTOR_23, 1);

UART uart3({
	{PC_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART3},		/* USART3_TX_PIN */
	{PC_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART3},		/* USART3_RX_PIN */
},
		USART3,
		DMA1,
		DMA1_Stream3,
		DMA_CHANNEL_4,
		DMA1_Stream1,
		DMA_CHANNEL_4,
		UART_HWCONTROL_NONE,
		9600,
		250
);


UART uart2({
	{PD_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART2},		/* USART3_TX_PIN */
	{PD_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART2},		/* USART3_RX_PIN */
},
		USART2,
		DMA1,
		DMA1_Stream6,		/* TX DMA stream */
		DMA_CHANNEL_4,		/* TX DMA channel */
		DMA1_Stream5,		/* RX DMA stream */
		DMA_CHANNEL_4,		/* RX DMA channel */
		UART_HWCONTROL_RTS_CTS,
		460800,
		250
);

void gyro_isr()
{
	if (gyro_int2) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t msg = 1;
		isr_ts.time_stamp();
		xQueueSendFromISR(hGyroQueue, &msg, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(true);
	}
}

void dronestate_boot_config(DroneState& state)
{
	rexjson::value bootconfig;

	try {
		bootconfig.read((char*)configdata.mem_, configdata.size());
		try { state.accelerometer_adjustment_ = matrix_from_json_value<float, 3, 1>(bootconfig["accelerometer_adjustment"]); } catch (std::exception& e) {}
		try { state.kp_ = bootconfig["kp"].get_real(); } catch (std::exception& e) {}
		try { state.ki_ = bootconfig["ki"].get_real(); } catch (std::exception& e) {}
		try { state.kd_ = bootconfig["kd"].get_real(); } catch (std::exception& e) {}
		try { state.yaw_kp_ = bootconfig["yaw_kp"].get_real(); } catch (std::exception& e) {}
		try { state.yaw_bias_ = bootconfig["yaw_bias"].get_real(); } catch (std::exception& e) {}
		try { state.pitch_bias_ = bootconfig["pitch_bias"].get_real(); } catch (std::exception& e) {}
		try { state.roll_bias_ = bootconfig["roll_bias"].get_real(); } catch (std::exception& e) {}
		try { state.yaw_ki_ = bootconfig["yaw_ki"].get_real(); } catch (std::exception& e) {}
		try { state.yaw_kd_ = bootconfig["yaw_kd"].get_real(); } catch (std::exception& e) {}
		try { state.gyro_factor_ = bootconfig["gyro_factor"].get_real(); } catch (std::exception& e) {}
		try { state.accelerometer_correction_period_ = bootconfig["accelerometer_correction_period"].get_real(); } catch (std::exception& e) {}
	} catch (std::exception& e) {
	}

}

void bmp180_task(void *pvParameters)
{
	(void)pvParameters;

	vTaskDelay(600 / portTICK_RATE_MS);

	I2CMaster i2c(I2C1, 400000, I2C_DUTYCYCLE_2, I2C_ADDRESSINGMODE_7BIT, 25, {
				{PB_8, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FAST, GPIO_AF4_I2C1},
				{PB_9, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FAST, GPIO_AF4_I2C1},
		});
	BMP180 bmp(i2c);
	TimeStamp led_toggle_ts;

	Bmp180Reader* bmp_reader = new Bmp180Reader(bmp);

	bmp_reader->calibrate();
	while (1) {
		if (led_toggle_ts.elapsed() > TimeSpan::from_seconds(1)) {
			led_toggle_ts.time_stamp();
			ledusb.toggle();
		}

		try {
			drone_state->altitude_ = bmp_reader->altitude_meters(true);
			drone_state->pressure_hpa_ = bmp_reader->pressure_hpa();
			drone_state->temperature_ = bmp_reader->temperature_celsius(true);
		} catch (std::exception& e) {
		}

		vTaskDelay(10 / portTICK_RATE_MS);
	}
}

void battery_task(void *pvParameters)
{
	(void)pvParameters;

	ADCHandle adc(ADC1, ADC_CHANNEL_9, PB_1);
	while (1) {
		uint32_t adc_val = adc.read_value();
		if (adc_val != ADCHandle::INVALID_CONV_VALUE) {
			float battery = (float)adc_val/(float)adc.max_value() * 3.3f * 125.1f / 25.1f;
			drone_state->battery_level_ = battery;
		}
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void main_task(void *pvParameters)
{
	(void) pvParameters;

	vTaskDelay(500 / portTICK_RATE_MS);

	SPIMaster spi5(SPI5, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{PF_7, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_SCK_PIN */
				{PF_8, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_MISO_PIN */
				{PF_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},			/* DISCOVERY_SPIx_MOSI_PIN */
			}, {
				{PC_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},					/* GYRO_CS_PIN */
				{PD_7, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},					/* ACCEL_CS_PIN */
			});
	L3GD20 gyro(spi5, 0);
	LSM303D accel(spi5, 1);
	uint8_t gyr_wtm = 3;
	uint8_t acc_wtm = 17;
	uint8_t bias_iterations = static_cast<uint8_t>(800);
	L3GD20::AxesDPS_t gyr_axes;
	LSM303D::AxesAcc_t acc_axes;
	attitudetracker att;
	TimeStamp console_update_time;
	TimeStamp sample_dt;
	TimeSpan ctx_switch_time;
	TimeStamp led_toggle_ts;
	FlightControl flight_ctl;
	UartRpcServer rpcserver(*drone_state, configdata);
	AccelLowPassFilter* accel_lpf = new AccelLowPassFilter();

	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA1_Stream6_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA1_Stream5_IRQn);
	uart2.uart_dmarx_start();

	printf("Priority Group: %lu\n", NVIC_GetPriorityGrouping());
	printf("SysTick_IRQn priority: %lu\n", NVIC_GetPriority(SysTick_IRQn) << __NVIC_PRIO_BITS);
	printf("configKERNEL_INTERRUPT_PRIORITY: %d\n", configKERNEL_INTERRUPT_PRIORITY);
	printf("configMAX_SYSCALL_INTERRUPT_PRIORITY: %d\n", configMAX_SYSCALL_INTERRUPT_PRIORITY);
	vTaskDelay(500 / portTICK_RATE_MS);

	gyro_int2.callback(gyro_isr);

	hGyroQueue = xQueueCreate(10, sizeof(uint32_t));
	vTaskDelay(500 / portTICK_RATE_MS);

	gyro.SetMode(L3GD20::NORMAL);
	gyro.SetFullScale(L3GD20::FULLSCALE_500);
	gyro.SetBDU(L3GD20::MEMS_ENABLE);
	gyro.SetWaterMark(gyr_wtm);
	gyro.FIFOModeEnable(L3GD20::FIFO_STREAM_MODE);
	gyro.SetInt2Pin(0);
	gyro.SetInt2Pin(L3GD20_WTM_ON_INT2_ENABLE| L3GD20_OVERRUN_ON_INT2_ENABLE);
	gyro.SetAxis(L3GD20_X_ENABLE|L3GD20_Y_ENABLE|L3GD20_Z_ENABLE);
	gyro.HPFEnable(L3GD20::MEMS_ENABLE);
	gyro.SetHPFMode(L3GD20::HPM_NORMAL_MODE_RES);
	gyro.SetHPFCutOFF(L3GD20::HPFCF_0);
	gyro.SetODR(L3GD20::ODR_760Hz_BW_35);

	accel.SetODR(LSM303D::ODR_400Hz);
	accel.SetFullScale(LSM303D::FULLSCALE_8);
	accel.SetAxis(LSM303D::X_ENABLE | LSM303D::Y_ENABLE | LSM303D::Z_ENABLE);
	accel.FIFOModeSet(LSM303D::FIFO_STREAM_MODE);
	accel.SetThreshold(acc_wtm);
	accel.SetInt2Pin(LSM303D_INT2_OVERRUN_ENABLE|LSM303D_INT2_FTH_ENABLE);
	accel.SetInt2Pin(0);

	vTaskDelay(500 / portTICK_RATE_MS);

	Vector3f gyr_bias;

	printf("Calibrating...\n");

	gyro.GetFifoAngRateDPS(&gyr_axes); // Drain the fifo
	for (int i = 0; i < bias_iterations; i++) {
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 50 ) ) {
		}
		gyro.GetFifoAngRateDPS(&gyr_axes);
		gyr_bias.at(0) += gyr_axes.AXIS_X;
		gyr_bias.at(1) += gyr_axes.AXIS_Y;
		gyr_bias.at(2) += gyr_axes.AXIS_Z;
	}
	gyr_bias = gyr_bias / (float)bias_iterations;

	flight_ctl.start_receiver();
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA1_Stream3_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA1_Stream1_IRQn);

	uart3.uart_dmarx_start();
	gpspwr.write(0);

	/*
	 * Apply the boot configuration from flash memory.
	 */
	dronestate_boot_config(*drone_state);

	// Infinite loop
	while (1) {
		uint32_t msg;

		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 50 ) ) {
		}
		ctx_switch_time = isr_ts.elapsed();
		drone_state->dt_ = sample_dt.elapsed();
		sample_dt.time_stamp();

		uint8_t gyr_samples = gyro.GetFifoSourceReg() & 0x1F;
//		uint8_t acc_samples = accel.GetFifoSourceFSS();

		att.accelerometer_correction_period(drone_state->accelerometer_correction_period_);
		static const Matrix3f gyro_align(-1,0,0,0,-1,0,0,0,1);
		if (gyr_samples >= gyr_wtm) {
			gyro.GetFifoAngRateDPS(&gyr_axes);
			drone_state->gyro_raw_ = gyro_align * (Vector3f(gyr_axes.AXIS_X, gyr_axes.AXIS_Y, gyr_axes.AXIS_Z) - gyr_bias);
			drone_state->gyro_ = drone_state->gyro_raw_ * drone_state->gyro_factor_;
			att.track_gyroscope(DEG2RAD(drone_state->gyro_), drone_state->dt_.seconds_float());
		}

		if (accel.GetFifoAcc(&acc_axes)) {
			drone_state->accel_raw_ = Vector3f(acc_axes.AXIS_X, acc_axes.AXIS_Y, acc_axes.AXIS_Z);
			Vector3f accel_adjusted = drone_state->accel_raw_ + drone_state->accelerometer_adjustment_;
			drone_state->accel_ = accel_lpf->do_filter(accel_adjusted.normalize());
		}
		att.track_accelerometer(drone_state->accel_, drone_state->dt_.seconds_float());
		drone_state->attitude_ = att.get_attitude();

		flight_ctl.update_state(*drone_state);
		flight_ctl.send_throttle_to_motors();

		if (console_update_time.elapsed() > TimeSpan::from_milliseconds(300)) {
			console_update_time.time_stamp();
			printf("Gyro      : %5.3f %5.3f %5.3f\n", drone_state->gyro_.at(0), drone_state->gyro_.at(1), drone_state->gyro_.at(2));
			printf("Accel     : %5.3f %5.3f %5.3f\n", drone_state->accel_.at(0), drone_state->accel_.at(1), drone_state->accel_.at(2));
			printf("dT        : %lu uSec\n", (uint32_t)drone_state->dt_.microseconds());
			printf("Q         : %5.3f %5.3f %5.3f %5.3f\n", drone_state->attitude_.w, drone_state->attitude_.x, drone_state->attitude_.y,
					drone_state->attitude_.z);
			QuaternionF tq = flight_ctl.target_q();
			printf("Target Q  : %5.3f %5.3f %5.3f %5.3f\n", tq.w, tq.x, tq.y, tq.z);
			printf("Throttle  : %.8f\n", flight_ctl.base_throttle().get());
			printf("Motors    : %1.3f %1.3f %1.3f %1.3f\n", drone_state->motors_.at(0,0), drone_state->motors_.at(1,0),
					drone_state->motors_.at(2,0), drone_state->motors_.at(3,0));
			printf("Altit, m  : %5.3f\n", drone_state->altitude_.meters());
			printf("Temper, C :%5.1f\n", drone_state->temperature_);

			//printf("Torq :  %1.3f %1.3f %1.3f\n", state.pid_torque_.at(0,0), state.pid_torque_.at(1,0),
				//	state.pid_torque_.at(2,0));
			printf("Servo      : %s\n", flight_ctl.servo().is_started() ? "armed" : "disarmed");
			if (!drone_state->alarm_.is_none()) {
				printf("%s %s, data: %d, @%5.3f sec\n", drone_state->alarm_.to_string(),
						drone_state->alarm_.severity_to_string(), (int)drone_state->alarm_.data(),
						drone_state->alarm_.when().seconds_float());
			}
			printf("\n");

#if 0
			memset(buf, 0, sizeof(buf));
			size_t retsize = uart3.receive((uint8_t*)buf, sizeof(buf));
			if (retsize) {
				printf("GPS: %s\n", buf);
			}
#endif

		}

#if 0
		if (led_toggle_ts.elapsed() > TimeSpan::from_seconds(1)) {
			led_toggle_ts.time_stamp();
			ledusb.toggle();
		}
#endif

		rpcserver.jsonrpc_request_handler(&uart2);
	}
}

extern void relocate_interrupt_table();

int main(int argc, char* argv[])
{
	uint32_t freq = HAL_RCC_GetSysClockFreq();
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();

	(void)argc;
	(void)argv;

	relocate_interrupt_table();

	/*
	 * Disable the SysTick_IRQn and clean the priority
	 * and let the scheduler configure and enable it.
	 */
	NVIC_DisableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 0);

	TimeStamp::init();
	colibri::UartTrace::init(115200*2);
	drone_state = new DroneState();

	printf("Starting main_task:, CPU freq: %lu, PCLK1 freq: %lu, PCLK2 freq: %lu\n",
			freq, pclk1, pclk2);

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

	vTaskStartScheduler(); // this call will never return
}

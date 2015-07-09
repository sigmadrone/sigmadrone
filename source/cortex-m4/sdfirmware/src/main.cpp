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
#include "attitudetracker.h"
#include "hwtimer.h"
#include "pwmencoder.h"
#include "pwmdecoder.h"
#include "timestamp.h"
#include "rexjson.h"
#include "rexjson++.h"
#include "tm_stm32f4_ili9341.h"
#include "colibripwm.h"
#include "colibritrace.h"
#include "rcreceiver.h"
#include "rcvalueconverter.h"
#include "servocontroller.h"

void* __dso_handle = 0;
extern unsigned int __relocated_vectors;

DigitalOut ledusb(PC_4);
//DigitalOut module_rstn(PA_4, DigitalOut::OutputDefault, DigitalOut::PullDefault, 1);
//DigitalOut module_onoff(PA_7, DigitalOut::OutputDefault, DigitalOut::PullDefault, 0);
DigitalOut gpspwr(PB_0, DigitalOut::OutputDefault, DigitalOut::PullDefault, 1);
DigitalIn gyro_int2(PA_2, DigitalIn::PullNone, DigitalIn::InterruptRising);
DigitalIn button(USER_BUTTON, DigitalIn::PullNone, DigitalIn::InterruptRising);
UART uart({
	{PA_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART1},		/* USART1_TX_PIN */
	{PA_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART1},		/* USART1_RX_PIN */
});

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
		250,
		9600
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
		250,
		9600
);


TaskHandle_t hMain;
QueueHandle_t hGyroQueue;
TimeStamp isr_ts;

extern "C" void __malloc_lock(void)
{
	vPortEnterCritical();
}

extern "C" void __malloc_unlock(void)
{
	vPortExitCritical();
}


extern "C" void EXTI0_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(0);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

extern "C" void EXTI2_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(2);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

extern "C" void EXTI4_IRQHandler(void)
{
	SPISlave::spi_chipselect_handler(4);
}

extern "C" void DMA2_Stream0_IRQHandler(void)
{
	SPISlave::spi_dmarx_handler(4);
}

extern "C" void DMA2_Stream1_IRQHandler(void)
{
	SPISlave::spi_dmatx_handler(4);
}

extern "C" void DMA2_Stream5_IRQHandler(void)
{
	UART::uart_dmarx_handler(1);
}

extern "C" void DMA2_Stream7_IRQHandler(void)
{
	UART::uart_dmatx_handler(1);
}

extern "C" void USART1_IRQHandler(void)
{
	UART::uart_irq_handler(1);
}

extern "C" void DMA1_Stream1_IRQHandler(void)
{
	UART::uart_dmarx_handler(3);
}

extern "C" void DMA1_Stream3_IRQHandler(void)
{
	UART::uart_dmatx_handler(3);
}

extern "C" void USART3_IRQHandler(void)
{
	UART::uart_irq_handler(3);
}

extern "C" void DMA1_Stream5_IRQHandler(void)
{
	UART::uart_dmarx_handler(2);
}

extern "C" void DMA1_Stream6_IRQHandler(void)
{
	UART::uart_dmatx_handler(2);
}

extern "C" void USART2_IRQHandler(void)
{
	UART::uart_irq_handler(2);
}

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

void secondary_task(void *pvParameters)
{
	// Infinite loop
	printf("Secondary task...\n");
	while (1) {
		vTaskDelay(250 / portTICK_RATE_MS);
	}
}

/* Definition for SPIx's NVIC */
#define SPIx_DMA_TX_IRQn                 DMA2_Stream1_IRQn
#define SPIx_DMA_RX_IRQn                 DMA2_Stream0_IRQn

void uart_tx_task(void *pvParameters)
{
	int i = 0;
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA2_Stream7_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA2_Stream5_IRQn);
	uart.uart_dmarx_start();
	HAL_Delay(7000);
	rexjson::value v = rexjson::object();
	v["UART"] = rexjson::object();
	while (1) {
		v["UART"]["message"] = "************ Test **********";
		v["UART"]["serial"] = i++;
		std::string str = v.write(false) + "\n";
		const char *bufptr = str.c_str();
		size_t size = str.length();
		size_t ret = 0;
		while (size) {
			ret = uart.transmit((uint8_t*)bufptr, size);
			size -= ret;
			bufptr += ret;
		}
		HAL_Delay(250);
	}
}

void uart2_tx_task(void *pvParameters)
{
	int i = 0;
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA1_Stream6_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA1_Stream5_IRQn);
	uart2.uart_dmarx_start();
	HAL_Delay(7000);
	rexjson::value v = rexjson::object();
	v["UART"] = rexjson::object();
	while (1) {
		v["UART"]["message"] = "************ Test **********";
		v["UART"]["serial"] = i++;
		std::string str = v.write(false) + "\r\n";
		const char *bufptr = str.c_str();
		size_t size = str.length();
		size_t ret = 0;
		while (size) {
			ret = uart2.transmit((uint8_t*)bufptr, size);
			size -= ret;
			bufptr += ret;
		}
		HAL_Delay(250);
	}
}

void spi_slave_task(void *pvParameters)
{
	char buf[128];
	unsigned int i = 0;
	printf("SPI Slave task...\n");
	SPISlave spi4({
				{PE_4, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_NSS_PIN */
				{PE_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_SCK_PIN */
				{PE_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_MISO_PIN */
				{PE_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_MOSI_PIN */
			}, 0);

	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA2_Stream1_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA2_Stream0_IRQn);
	HAL_NVIC_SetPriority(EXTI4_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	spi4.start();
	while (1) {
		HAL_Delay(50);
		snprintf(buf, sizeof(buf), "SPI: %d", i++);
		spi4.transmit((uint8_t*)buf, strlen(buf) + 1);
	}
}

#if 0
struct PwmDecoderCallback {
	PwmDecoderCallback(uint32_t id) :
			decoder_(colibri::PWM_RX_CONSTS[id].timer_id_,
					colibri::PWM_RX_CONSTS[id].pin_,
					TimeSpan::from_milliseconds(30),
					FunctionPointer(this, &PwmDecoderCallback::callback)), id_(id) {}
	void callback(void) {
		printf("PWM %lu: period %lu uS, duty: %.4f %lu uS\n", id_,
				(uint32_t)decoder_.decoded_period().microseconds(),
				decoder_.duty_cycle_rel(),
				(uint32_t)decoder_.duty_cycle().microseconds());
	}
	void start_decoder() {
		decoder_.start_on_ch1_ch2();
	}
	uint32_t decoder_id() { return id_; }

private:
	PwmDecoderCallback(const PwmDecoderCallback&);

	PwmDecoder decoder_;
	uint32_t id_;
};

static PwmEncoder pwmEncoder1(colibri::PWM_OUT_TIMER_1, TimeSpan::from_microseconds(2500),
		{colibri::PWM_OUT_PINS_1_4}, {1, 2, 3, 4});

static PwmEncoder pwmEncoder2(colibri::PWM_OUT_TIMER_2, TimeSpan::from_milliseconds(2),
		colibri::PWM_OUT_PINS_5_8, {1, 2, 3, 4});

void tim10_isr() {
	static float duty_cycle[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
	static int index = 0;
	for (uint32_t i = 0; i < 4; ++i) {
		pwmEncoder1.set_duty_cycle(i, duty_cycle[index]);
		pwmEncoder2.set_duty_cycle(i, duty_cycle[index]);
	}
	index = (index + 1) % (sizeof(duty_cycle)/sizeof(duty_cycle[1]));
}

void StartPwmTest() {
	static HwTimer tim10(HwTimer::TIMER_10, TimeSpan::from_seconds(2),
				Frequency::from_kilohertz(10000), FunctionPointer(tim10_isr));

	tim10.start();

	static std::vector<PwmDecoderCallback*> pwm_callbacks;
	for (uint32_t i = 0; i < 4; ++i) {
		pwm_callbacks.push_back(new PwmDecoderCallback(i));
	}

	for (auto decoder: pwm_callbacks) {
		decoder->start_decoder();
	}

	pwmEncoder1.start();
	pwmEncoder2.start();
	for (uint32_t i = 0; i < 4; ++i) {
		pwmEncoder1.set_duty_cycle(i, TimeSpan::from_milliseconds(1));
		pwmEncoder2.set_duty_cycle(i, TimeSpan::from_milliseconds(1));
	}
}
#endif

class FlightControl
{
public:
	FlightControl() : rc_receiver_(colibri::PWM_RX_CONSTS,
			FunctionPointer(this, &FlightControl::rc_callback)),
			ch_mapper_({RC_CHANNEL_THROTTLE, RC_CHANNEL_RUDDER, RC_CHANNEL_ELEVATOR, RC_CHANNEL_AILERON}),
			rc_values_(ch_mapper_, rc_receiver_) ,
			servo_ctrl_({colibri::PWM_TX_1_4}, Frequency::from_hertz(400)) {
	}
	void start_receiver() { rc_receiver_.start(); }
	void stop_receiver() { rc_receiver_.stop(); }
	QuaternionF target_q() const { return rc_values_.target_quaternion(); }
	Throttle base_throttle() const { return rc_values_.base_throttle(); }
	void set_throttle(const std::vector<Throttle>& thrVec) {
		PwmPulse pulse(TimeSpan::from_milliseconds(1), TimeSpan::from_milliseconds(2));
		for (size_t i = 0; i < thrVec.size(); ++i) {
			Throttle thr = thrVec[i];
			TimeSpan ts = pulse.to_timespan(thr.get());
			servo_ctrl_.set_pwm_pulse(i, pulse.to_timespan(thrVec[i].get()));
		}
	}
	void update_throttle() {
		set_throttle({base_throttle(), base_throttle(), base_throttle(), base_throttle()});
	}
	void start_servo() {
		servo_ctrl_.start();
		// TODO: to be done when signal is received
		servo_ctrl_.arm_motors();
	}

private:
	void rc_callback() {
		rc_values_.update();
	}
private:
	RcReceiver rc_receiver_;
	RcChannelMapper ch_mapper_;
	RcValueConverter rc_values_;
	ServoController servo_ctrl_;
};

#define portNVIC_SYSPRI2_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )

int lcd_init(void)
{

	//Initialize ILI9341
	TM_ILI9341_Init();
	//Rotate LCD for 90 degrees
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	//FIll lcd with color
	TM_ILI9341_Fill(ILI9341_COLOR_WHITE);
	//Draw white circle
	TM_ILI9341_DrawCircle(60, 60, 40, ILI9341_COLOR_GREEN);
	//Draw red filled circle
	TM_ILI9341_DrawFilledCircle(60, 60, 35, ILI9341_COLOR_RED);
	//Draw blue rectangle
	TM_ILI9341_DrawRectangle(120, 20, 220, 100, ILI9341_COLOR_BLUE);
	//Draw black filled rectangle
	TM_ILI9341_DrawFilledRectangle(130, 30, 210, 90, ILI9341_COLOR_BLACK);
	//Draw line with custom color 0x0005
	TM_ILI9341_DrawLine(10, 120, 310, 120, 0x0005);

	//Put string with black foreground color and blue background with 11x18px font
	TM_ILI9341_Puts(65, 130, "STM32F4 Colibri", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	//Put string with black foreground color and blue background with 11x18px font
	TM_ILI9341_Puts(60, 150, "ILI9341 LCD Module", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	//Put string with black foreground color and red background with 11x18px font
	TM_ILI9341_Puts(190, 225, "www.sigmadrone.org", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	TM_ILI9341_Fill(ILI9341_COLOR_WHITE);

	return 0;
}

void DisplayStringAt(uint16_t x, uint16_t y, const char *str)
{
	return;
	TM_ILI9341_Puts(x, y, str, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void main_task(void *pvParameters)
{
	char buf[256];
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
	uint8_t gyr_wtm = 20;
	uint8_t acc_wtm = 17;
	uint8_t bias_iterations = 10;
	L3GD20::AxesDPS_t gyr_axes;
	LSM303D::AxesAcc_t acc_axes;
	QuaternionF q;
	attitudetracker att;

	printf("Priority Group: %lu\n", NVIC_GetPriorityGrouping());
	printf("SysTick_IRQn priority: %lu\n", NVIC_GetPriority(SysTick_IRQn) << __NVIC_PRIO_BITS);
	printf("configKERNEL_INTERRUPT_PRIORITY: %d\n", configKERNEL_INTERRUPT_PRIORITY);
	printf("configMAX_SYSCALL_INTERRUPT_PRIORITY: %d\n", configMAX_SYSCALL_INTERRUPT_PRIORITY);
	vTaskDelay(500 / portTICK_RATE_MS);

#if 0
	lcd_init();
#endif

	gyro_int2.callback(gyro_isr);

//	module_rstn.write(0);

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
	gyro.SetODR(L3GD20::ODR_380Hz_BW_25);

	accel.SetODR(LSM303D::ODR_400Hz);
	accel.SetFullScale(LSM303D::FULLSCALE_8);
	accel.SetAxis(LSM303D::X_ENABLE | LSM303D::Y_ENABLE | LSM303D::Z_ENABLE);
	accel.FIFOModeSet(LSM303D::FIFO_STREAM_MODE);
	accel.SetThreshold(acc_wtm);
	accel.SetInt2Pin(LSM303D_INT2_OVERRUN_ENABLE|LSM303D_INT2_FTH_ENABLE);
	accel.SetInt2Pin(0);

	vTaskDelay(500 / portTICK_RATE_MS);

	Vector3f gyr_bias;
	char disp[128] = {0};

	printf("Calibrating...\n");
	sprintf(disp,"Calibrating...");
	DisplayStringAt(0, 10, disp);

	gyro.GetFifoAngRateDPS(&gyr_axes); // Drain the fifo
	for (int i = 0; i < bias_iterations; i++) {
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {
		}
		gyro.GetFifoAngRateDPS(&gyr_axes);
		gyr_bias.at(0) += gyr_axes.AXIS_X;
		gyr_bias.at(1) += gyr_axes.AXIS_Y;
		gyr_bias.at(2) += gyr_axes.AXIS_Z;
	}
	gyr_bias = gyr_bias / (float)bias_iterations;
	TimeStamp console_update_time;
	TimeStamp sample_dt;
	TimeSpan ctx_switch_time;
	TimeStamp led_toggle_ts;

	FlightControl flight_ctl;
	flight_ctl.start_receiver();
	flight_ctl.start_servo();

	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA1_Stream3_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA1_Stream1_IRQn);

	uart3.uart_dmarx_start();
	gpspwr.write(0);

	// Infinite loop
	while (1) {
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {
		}
		ctx_switch_time = isr_ts.elapsed();

		uint8_t gyr_samples = gyro.GetFifoSourceReg() & 0x1F;
		uint8_t acc_samples = accel.GetFifoSourceFSS();

		if (gyr_samples >= gyr_wtm)
			gyro.GetFifoAngRateDPS(&gyr_axes);
		if (acc_samples > acc_wtm)
			accel.GetFifoAcc(&acc_axes);

		TimeSpan dt = sample_dt.elapsed();

		Vector3f gyr_data = Vector3f(gyr_axes.AXIS_X, gyr_axes.AXIS_Y, gyr_axes.AXIS_Z) - gyr_bias;
		Vector3f acc_data = Vector3f(acc_axes.AXIS_X, acc_axes.AXIS_Y, acc_axes.AXIS_Z).normalize();
		static const Matrix3f gyro_align(-1,0,0,0,-1,0,0,0,1);
		gyr_data = gyro_align * gyr_data * 1.0;
		att.track_gyroscope(DEG2RAD(gyr_data), dt.seconds_float());
		att.track_accelerometer(acc_data, dt.seconds_float());
		q = att.get_attitude();

		if (console_update_time.elapsed() > TimeSpan::from_milliseconds(200)) {
			console_update_time.time_stamp();

			printf("Gyro: %5.3f %5.3f %5.3f\n", gyr_data.at(0), gyr_data.at(1), gyr_data.at(2));
			printf("Acc : %5.3f %5.3f %5.3f\n", acc_data.at(0), acc_data.at(1), acc_data.at(2));
			printf("Q   : %5.3f %5.3f %5.3f %5.3f\n", q.w, q.x, q.y, q.z);
			printf("Thro: %.8f\n", flight_ctl.base_throttle().get());
			QuaternionF tq = flight_ctl.target_q();
			printf("TQ  : %5.3f %5.3f %5.3f %5.3f\n",  tq.w, tq.x, tq.y, tq.z);
			printf("\n");
			flight_ctl.update_throttle();

			try {
				memset(buf, 0, sizeof(buf) - 1);
				size_t retsize = uart2.receive((uint8_t*)buf, sizeof(buf));
				if (retsize) {
					printf("UART2: %s\n", buf);
					rexjson::value v = rexjson::read(buf);
					sprintf(disp, "serial : %d      ", v["UART"]["serial"].get_int());
//					printf("%s\n", disp);
//					DisplayStringAt(0, 80, disp);
				}
			} catch (std::exception& e) {
				printf("exception: %s\n", e.what());
				uart2.clear();
			}

#if 0
			memset(buf, 0, sizeof(buf));
			size_t retsize = uart3.receive((uint8_t*)buf, sizeof(buf));
			if (retsize) {
				printf("GPS: %s\n", buf);
			}
#endif

#if 0
			sprintf(disp,"SAMPLES: %d           ", gyr_samples);
			DisplayStringAt(0, 60, disp);

			sprintf(disp,"PWM: %u mS", (unsigned)pwmDecoder.decoded_period().milliseconds());
			DisplayStringAt(0, 100, disp);
			sprintf(disp,"PWM duty: %1.3f", pwmDecoder.duty_cycle_rel());
			DisplayStringAt(0, 120, disp);
			sprintf(disp,"ACCL X: %6.2f", acc_axes.AXIS_X);
			DisplayStringAt(0, 100, disp);
			sprintf(disp,"ACCL Y: %6.2f", acc_axes.AXIS_Y);
			DisplayStringAt(0, 120, disp);
			sprintf(disp,"ACCL Z: %6.2f", acc_axes.AXIS_Z);
			DisplayStringAt(0, 140, disp);
			sprintf(disp,"SAMPLES: %d           ", acc_samples);
			DisplayStringAt(0, 160, disp);

			sprintf(disp,"CTXSW: %u uS          ", (unsigned int)ctx_switch_time.microseconds());
			DisplayStringAt(0, 140, disp);

			sprintf(disp,"dT: %u uS             ", (unsigned int)dt.microseconds());
			DisplayStringAt(0, 160, disp);

			sprintf(disp,"Attitude:             ");
			DisplayStringAt(0, 180, disp);
			sprintf(disp,"W:      %6.3f              ", q.w);
			DisplayStringAt(0, 200, disp);
			sprintf(disp,"X:      %6.3f              ", q.x);
			DisplayStringAt(0, 220, disp);
			sprintf(disp,"Y:      %6.3f              ", q.y);
			DisplayStringAt(0, 240, disp);
			sprintf(disp,"Z:      %6.3f              ", q.z);
			DisplayStringAt(0, 260, disp);

			memset(disp, 0, sizeof(disp));
			spi5.read(2, (uint8_t*)disp, 15);
			DisplayStringAt(0, 300, disp);
#endif
		}

		if (led_toggle_ts.elapsed() > TimeSpan::from_seconds(1)) {
			led_toggle_ts.time_stamp();
			ledusb.toggle();
		}

		sample_dt.time_stamp();
	}
}


void relocate_interrupt_table()
{
	volatile unsigned int* newtable = &__relocated_vectors;
	volatile unsigned int* oldtable = 0;

	for (size_t i = 0; i < 256; i++) {
		newtable[i] = oldtable[i];
	}

	newtable[SVCall_IRQn + 16] = (unsigned int)vPortSVCHandler;
	newtable[PendSV_IRQn + 16] = (unsigned int)xPortPendSVHandler;
	newtable[SysTick_IRQn + 16] = (unsigned int)xPortSysTickHandler;

	__DMB();
	SCB->VTOR = ((unsigned int)newtable);
	__DSB();
}





int main(int argc, char* argv[])
{
	uint32_t freq = HAL_RCC_GetSysClockFreq();
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();

	relocate_interrupt_table();

	/*
	 * Disable the SysTick_IRQn and clean the priority
	 * and let the scheduler configure and enable it.
	 */
	NVIC_DisableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 0);

	TimeStamp::init();
	colibri::UartTrace::init(115200*2);

	printf("Starting main_task:, CPU freq: %lu, PCLK1 freq: %lu, PCLK2 freq: %lu\n",
			freq, pclk1, pclk2);

	  /* Create tasks */
	xTaskCreate(
		main_task, /* Function pointer */
		"Task1", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 3UL, /* Task priority*/
		&hMain /* Task handle */
		);

#if 0
	xTaskCreate(
		secondary_task, /* Function pointer */
		"Task2", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);
#endif

#if 0
	xTaskCreate(
		spi_slave_task, /* Function pointer */
		"SPI Slave Task", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);

#endif
	xTaskCreate(
		uart_tx_task, /* Function pointer */
		"UART TX Task", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);

	xTaskCreate(
		uart2_tx_task, /* Function pointer */
		"UART2 TX Task", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);


//	vTaskList(buffer);
//	printf("Tasks: \n%s\n\n", buffer);
	vTaskStartScheduler();

	// Infinite loop
	while (1) {
		printf("Hello world, freq: %lu, f=%f\n", freq, 0.75);
	}
	// Infinite loop, never return.
}

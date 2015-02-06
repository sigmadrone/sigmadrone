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
#include "stm32f429i_discovery_lcd.h"
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
#include "jsmn.h"

void* __dso_handle = 0;

DigitalOut led1(USER_LED1);
DigitalOut led2(USER_LED2);
DigitalIn gyro_int2(PA_2, DigitalIn::PullNone, DigitalIn::InterruptRising);
DigitalIn button(USER_BUTTON, DigitalIn::PullNone, DigitalIn::InterruptRising);
UART uart({
	{PA_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART1},		/* USART1_TX_PIN */
	{PA_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF7_USART1},		/* USART1_RX_PIN */
});


TaskHandle_t hMain;
QueueHandle_t hGyroQueue;
TimeStamp isrTs;

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

void gyro_isr()
{
	if (gyro_int2) {
		isrTs.time_stamp();
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t msg = 1;
		xQueueSendFromISR(hGyroQueue, &msg, &xHigherPriorityTaskWoken);
	}
}

void secondary_task(void *pvParameters)
{
	// Infinite loop
	trace_printf("Secondary task...\n");
	while (1) {
		vTaskDelay(250 / portTICK_RATE_MS);
	}
}

/* Definition for SPIx's NVIC */
#define SPIx_DMA_TX_IRQn                 DMA2_Stream1_IRQn
#define SPIx_DMA_RX_IRQn                 DMA2_Stream0_IRQn

void uart_tx_task(void *pvParameters)
{
	char buf[128];
	int i = 0;
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (DMA2_Stream7_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (DMA2_Stream5_IRQn);
	uart.uart_dmarx_start();
	HAL_Delay(7000);
	while (1) {
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf) - 1, "{\"UART\" : {\"message\" : \"************ Test **********\", \"serial\":%7d}}\n", i++);
		size_t size = strlen(buf);
		uint8_t *bufptr = (uint8_t*)buf;
		size_t ret = 0;
		while (size) {
			ret = uart.transmit((uint8_t*)bufptr, size);
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
	trace_printf("SPI Slave task...\n");
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


//void HAL_Delay(__IO uint32_t delay)
//{
//	vTaskDelay(delay / portTICK_RATE_MS);
//}

#define LCD_FRAME_BUFFER_LAYER0                  0xC0130000
#define LCD_FRAME_BUFFER_LAYER1                  0xC0000000
#define CONVERTED_FRAME_BUFFER                   0xC0260000

#if 0
void init_lcd()
{
	/*##-1- Initialize the LCD #################################################*/
	/* Initialize the LCD */
	BSP_LCD_Init();

	/* Initialise the LCD Layers */
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);

	/* Set LCD Foreground Layer  */
	BSP_LCD_SelectLayer(1);

	BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

	/* Clear the LCD */
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_Clear(LCD_COLOR_WHITE);


	/* Set the LCD Text Color */
	BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

	/* Display LCD messages */
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)"STM32F429I BSP", CENTER_MODE);

	BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"Hello world!", CENTER_MODE);

	trace_printf("LCD %d by %d pixels\n", BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	vTaskDelay(200 / portTICK_RATE_MS);

	BSP_LCD_Clear(LCD_COLOR_WHITE);
}
#endif

void init_lcd()
{
	/*##-1- Initialize the LCD #################################################*/
	/* Initialize the LCD */
	BSP_LCD_Init();

#if 0
	/* Initialise the LCD Layers */
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);

	/* Set LCD Foreground Layer  */
	BSP_LCD_SelectLayer(1);
#endif

	for (uint8_t layer = 0; layer < 2; ++layer)
	{
		BSP_LCD_LayerDefaultInit(layer, (uint32_t) LCD_FRAME_BUFFER +
				(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4)*layer);
		BSP_LCD_SetLayerVisible(layer, DISABLE);
		BSP_LCD_SelectLayer(layer);

		BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

		/* Clear the LCD */
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_Clear(LCD_COLOR_WHITE);

		/* Set the LCD Text Color */
		BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
	}

	BSP_LCD_SelectLayer(1);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)"STM32F429I BSP", CENTER_MODE);
	BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"Hello world!", CENTER_MODE);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	vTaskDelay(1000 / portTICK_RATE_MS);

	BSP_LCD_SelectLayer(0);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)"LAYER 0", CENTER_MODE);
	BSP_LCD_SetLayerVisible(1, DISABLE);
	BSP_LCD_SetLayerVisible(0, ENABLE);


	trace_printf("LCD %d by %d pixels\n", BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	vTaskDelay(200 / portTICK_RATE_MS);

	BSP_LCD_Clear(LCD_COLOR_WHITE);
}


void pwm_decoder_callback();

PwmEncoder pwmEncoder(HwTimer::TIMER_1, TimeSpan::from_seconds(2), {PA_8}, {1});
PwmDecoder pwmDecoder(HwTimer::TIMER_4, PB_7, TimeSpan::from_seconds(3),
		FunctionPointer(pwm_decoder_callback));

void pwm_decoder_callback() {
	float duty_cycle = pwmDecoder.duty_cycle_rel();
	trace_printf("PWM decoded: period %d mS, duty cycle %.4f\n",
			(uint32_t)pwmDecoder.decoded_period().milliseconds(),
			duty_cycle);
}

void tim3_isr() {
	static float duty_cycle[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
	static int index = 0;
	static TimeStamp tim3_ts;
	TimeSpan elapsed = tim3_ts.elapsed();
	tim3_ts.time_stamp();
	trace_printf("==> tim3 interrupt, elapsed %u us\n",
			(unsigned int)elapsed.microseconds());
	led2.toggle();
	pwmEncoder.set_duty_cycle(1, duty_cycle[index]);
	index = (index + 1) % (sizeof(duty_cycle)/sizeof(duty_cycle[1]));
}

#define portNVIC_SYSPRI2_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )

void main_task(void *pvParameters)
{
	char buf[256];
	vTaskDelay(500 / portTICK_RATE_MS);

	HwTimer tim3(HwTimer::TIMER_3, TimeSpan::from_milliseconds(5000),
			Frequency::from_kilohertz(10), FunctionPointer(tim3_isr));
	tim3.start();

	/*
	 * NOTE: for this particular example in order for the pwmDecoder to function, then
	 * PA_8 and PB_7 must be connected. pwmEncoder outputs to PA_8 and pwmDecoder
	 * receives its input on PB_7.
	 */
	pwmDecoder.start();
	pwmEncoder.start();
	pwmEncoder.set_duty_cycle(1, TimeSpan::from_milliseconds(1000));

	SPIMaster spi5(SPI5, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{PF_7, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_SCK_PIN */
				{PF_8, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_MISO_PIN */
				{PF_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},			/* DISCOVERY_SPIx_MOSI_PIN */
			}, {
				{PC_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},					/* GYRO_CS_PIN */
				{PG_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},					/* ACCEL_CS_PIN */
				{PG_3, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_MEDIUM, 0},					/* SLAVE_CS_PIN */
			});
	L3GD20 gyro(spi5, 0);
	LSM303D accel(spi5, 1);
	uint8_t gyr_wtm = 20;
	uint8_t acc_wtm = 17;
	uint8_t bias_iterations = 10;
	L3GD20::AxesDPS_t gyr_axes;
	LSM303D::AxesAcc_t acc_axes;
	QuaternionD q;
	attitudetracker att;

	trace_printf("Priority Group: %u\n", NVIC_GetPriorityGrouping());
	trace_printf("SysTick_IRQn priority: %u\n", NVIC_GetPriority(SysTick_IRQn) << __NVIC_PRIO_BITS);
	trace_printf("configKERNEL_INTERRUPT_PRIORITY: %u\n", configKERNEL_INTERRUPT_PRIORITY);
	trace_printf("configMAX_SYSCALL_INTERRUPT_PRIORITY: %u\n", configMAX_SYSCALL_INTERRUPT_PRIORITY);
	vTaskDelay(500 / portTICK_RATE_MS);

	gyro_int2.callback(gyro_isr);
	init_lcd();

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

	vTaskDelay(1000 / portTICK_RATE_MS);
	// Infinite loop
	TickType_t ticks = 0, oldticks = 0;
	Vector3d gyr_bias;
	char disp[128] = {0};

	sprintf(disp,"Calibrating...");
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)disp, LEFT_MODE);

	gyro.GetFifoAngRateDPS(&gyr_axes); // Drain the fifo
	for (int i = 0; i < bias_iterations; i++) {
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {
		}
		gyro.GetFifoAngRateDPS(&gyr_axes);
		gyr_bias.at(0) += gyr_axes.AXIS_X;
		gyr_bias.at(1) += gyr_axes.AXIS_Y;
		gyr_bias.at(2) += gyr_axes.AXIS_Z;
		led1.toggle();
	}
	gyr_bias = gyr_bias / (double)bias_iterations;
	TimeStamp lcdUpdateTime;
	TimeStamp sampleDt;
	TimeSpan ctxSwitchTime;

	while (1) {
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {
		}
		ctxSwitchTime = isrTs.elapsed();

		uint8_t gyr_samples = gyro.GetFifoSourceReg() & 0x1F;
		uint8_t acc_samples = accel.GetFifoSourceFSS();

		if (gyr_samples >= gyr_wtm)
			gyro.GetFifoAngRateDPS(&gyr_axes);
		if (acc_samples > acc_wtm)
			accel.GetFifoAcc(&acc_axes);

		ticks = xTaskGetTickCount() - oldticks;
		TimeSpan dt = sampleDt.elapsed();

		Vector3d gyr_data = Vector3d(gyr_axes.AXIS_X, gyr_axes.AXIS_Y, gyr_axes.AXIS_Z) - gyr_bias;
		oldticks = xTaskGetTickCount();
		att.track_gyroscope(DEG2RAD(gyr_data), ticks * portTICK_PERIOD_MS / (float)1000.0);
		q = att.get_attitude();

		if (lcdUpdateTime.elapsed().milliseconds() > 500) {
			lcdUpdateTime.time_stamp();

			sprintf(disp,"GYRO X: %6.2f         ", gyr_data.at(0));
			BSP_LCD_DisplayStringAt(0, 00, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"GYRO Y: %6.2f         ", gyr_data.at(1));
			BSP_LCD_DisplayStringAt(0, 20, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"GYRO Z: %6.2f         ", gyr_data.at(2));
			BSP_LCD_DisplayStringAt(0, 40, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"SAMPLES: %d           ", gyr_samples);
			BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)disp, LEFT_MODE);

			memset(buf, 0, sizeof(buf));
			size_t retsize = uart.receive((uint8_t*)buf, sizeof(buf));
			if (retsize) {
				//trace_printf("%s", buf);
				BSP_LCD_DisplayStringAt(0, 80, (uint8_t*)buf, LEFT_MODE);
			}

			sprintf(disp,"CtxSw: %u uS", (unsigned int)ctxSwitchTime.microseconds());
			BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)disp, LEFT_MODE);
			ctxSwitchTime = TimeSpan::from_seconds(0);

			sprintf(disp,"dT: %u uS", (unsigned int)dt.microseconds());
			BSP_LCD_DisplayStringAt(0, 120, (uint8_t*)disp, LEFT_MODE);

#if 0
			sprintf(disp,"ACCL X: %6.2f", acc_axes.AXIS_X);
			BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"ACCL Y: %6.2f", acc_axes.AXIS_Y);
			BSP_LCD_DisplayStringAt(0, 120, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"ACCL Z: %6.2f", acc_axes.AXIS_Z);
			BSP_LCD_DisplayStringAt(0, 140, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"SAMPLES: %d           ", acc_samples);
			BSP_LCD_DisplayStringAt(0, 160, (uint8_t*)disp, LEFT_MODE);
#endif
			sprintf(disp,"Attitude:             ");
			BSP_LCD_DisplayStringAt(0, 180, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"W:      %6.2f              ", q.w);
			BSP_LCD_DisplayStringAt(0, 200, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"X:      %6.2f              ", q.x);
			BSP_LCD_DisplayStringAt(0, 220, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"Y:      %6.2f              ", q.y);
			BSP_LCD_DisplayStringAt(0, 240, (uint8_t*)disp, LEFT_MODE);
			sprintf(disp,"Z:      %6.2f              ", q.z);
			BSP_LCD_DisplayStringAt(0, 260, (uint8_t*)disp, LEFT_MODE);

			memset(disp, 0, sizeof(disp));
			spi5.read(2, (uint8_t*)disp, 15);
			BSP_LCD_DisplayStringAt(0, 300, (uint8_t*)disp, LEFT_MODE);
//			trace_printf("recved: %s\n", disp);
		}
		sampleDt.time_stamp();
		led1.toggle();
	}
}

int main(int argc, char* argv[])
{
	uint32_t freq = HAL_RCC_GetSysClockFreq();
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();

	/*
	 * Disable the SysTick_IRQn and clean the priority
	 * and let the scheduler configure and enable it.
	 */
	NVIC_DisableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 0);

	TimeStamp::Init();

	button.callback(&led1, &DigitalOut::toggle);
	trace_printf("Starting main_task:, CPU freq: %d, PCLK1 freq: %d, PCLK2 freq: %d\n",
			freq, pclk1, pclk2);

	  /* Create tasks */
	xTaskCreate(
		main_task, /* Function pointer */
		"Task1", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
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

	xTaskCreate(
		spi_slave_task, /* Function pointer */
		"SPI Slave Task", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);

	xTaskCreate(
		uart_tx_task, /* Function pointer */
		"UART TX Task", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);

//	vTaskList(buffer);
//	trace_printf("Tasks: \n%s\n\n", buffer);
	vTaskStartScheduler();

	// Infinite loop
	while (1) {
		trace_printf("Hello world, freq: %d, f=%f\n", freq, 0.75);
	}
	// Infinite loop, never return.
}


extern "C" void vApplicationTickHook(void)
{
#if 0
	{
		/* Just to verify that the interrupt nesting behaves as expected,
		 increment ulFPUInterruptNesting on entry, and decrement it on exit. */
		ulFPUInterruptNesting++;

		/* Fill the FPU registers with 0. */
		vRegTestClearFlopRegistersToParameterValue( 0UL );

		/* Trigger a timer 2 interrupt, which will fill the registers with a
		 different value and itself trigger a timer 3 interrupt.  Note that the
		 timers are not actually used.  The timer 2 and 3 interrupt vectors are
		 just used for convenience. */
		NVIC_SetPendingIRQ( TIM2_IRQn );

		/* Ensure that, after returning from the nested interrupts, all the FPU
		 registers contain the value to which they were set by the tick hook
		 function. */
		configASSERT( ulRegTestCheckFlopRegistersContainParameterValue( 0UL ) );

		ulFPUInterruptNesting--;
	}
#endif
}

extern "C" void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	 to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	 task.  It is essential that code added to this hook function never attempts
	 to block in any way (for example, call xQueueReceive() with a block time
	 specified, or call vTaskDelay()).  If the application makes use of the
	 vTaskDelete() API function (as this demo application does) then it is also
	 important that vApplicationIdleHook() is permitted to return to its calling
	 function, because it is the responsibility of the idle task to clean up
	 memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

extern "C" void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for (;;)
		;
}

extern "C" void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

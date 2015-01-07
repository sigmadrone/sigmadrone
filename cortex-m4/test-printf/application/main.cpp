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
#include "l3gd20_device.h"
#include "lsm303d.h"

void* __dso_handle = 0;

DigitalOut led1(USER_LED1);
DigitalOut led2(USER_LED2);
DigitalIn button(USER_BUTTON, DigitalIn::PullDown, DigitalIn::InterruptRising);
DigitalIn wtm(PA_2, DigitalIn::PullNone, DigitalIn::InterruptRisingFalling);
TaskHandle_t hMain;
QueueHandle_t hGyroQueue;

extern "C" void EXTI0_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(0);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

extern "C" void EXTI2_IRQHandler(void)
{
//	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(2);
	HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
//	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

void wtm_isr()
{
	led2 = wtm;
	if (wtm) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t msg = 1;
		xQueueSendFromISR(hGyroQueue, &msg, &xHigherPriorityTaskWoken);
//		xTaskResumeFromISR(hMain);
//		trace_printf("WTM isr...\n");
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

void HAL_Delay(__IO uint32_t delay)
{
	vTaskDelay(delay / portTICK_RATE_MS);
}

#define LCD_FRAME_BUFFER_LAYER0                  0xC0130000
#define LCD_FRAME_BUFFER_LAYER1                  0xC0000000
#define CONVERTED_FRAME_BUFFER                   0xC0260000

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

	vTaskDelay(2000 / portTICK_RATE_MS);

	BSP_LCD_Clear(LCD_COLOR_WHITE);
}

void main_task(void *pvParameters)
{
	SPIMaster spi5(SPI5, 0x2000, {
				{PF_7, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_SCK_PIN */
				{PF_8, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_MISO_PIN */
				{PF_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},			/* DISCOVERY_SPIx_MOSI_PIN */
				{PA_1, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, 0},						/* INT1 */
				{PA_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, 0},						/* INT2 */
			}, {
				{PC_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* GYRO_CS_PIN */
				{PG_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* ACCEL_CS_PIN */
			});
	L3GD20 gyro(spi5, 0);
	LSM303D accel(spi5, 1);
	float data[3] = {0, 0, 0};
	L3GD20::AxesRaw_t raw;
	LSM303D::AxesRaw_t accraw;
	init_lcd();
	wtm.callback(wtm_isr);

	hGyroQueue = xQueueCreate(10, sizeof(uint32_t));

	gyro.SetMode(L3GD20::L3GD20_NORMAL);
	gyro.SetODR(L3GD20::L3GD20_ODR_95Hz_BW_25);
	gyro.SetFullScale(L3GD20::L3GD20_FULLSCALE_500);
	gyro.SetBDU(L3GD20::MEMS_ENABLE);
	gyro.SetWaterMark(14);
	gyro.FIFOModeEnable(L3GD20::L3GD20_FIFO_STREAM_MODE);
	gyro.SetInt2Pin(L3GD20_WTM_ON_INT2_ENABLE| L3GD20_OVERRUN_ON_INT2_ENABLE);
	gyro.SetAxis(L3GD20_X_ENABLE|L3GD20_Y_ENABLE|L3GD20_Z_ENABLE);

//	accel.SetODR(ODR_100Hz);
//	accel.SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);

	// Infinite loop
	char disp[128] = {0};
	while (1) {
		uint8_t count = gyro.GetFifoSourceReg() & 0x1F;
		data[0] = data[1] = data[2] = 0.0;
		for (uint8_t i = 0; i < count; i++) {
			gyro.GetAngRateRaw(&raw);
			data[0] += raw.AXIS_X * 500.0 / 32768.0 / count;
			data[1] += raw.AXIS_Y * 500.0 / 32768.0 / count;
			data[2] += raw.AXIS_Z * 500.0 / 32768.0 / count;
		}
//		trace_printf("FIFO samples: %d, data: %f, %f, %f\n", count, data[0], data[1], data[2]);
//		u8_t id = 0;
//		accel.ReadReg8(WHO_AM_I, &id);
//		accel.GetAccAxesRaw(&accraw);
//		trace_printf("Accelerometer ID: 0x%x, raw: %d %d %d\n", id, accraw.AXIS_X, accraw.AXIS_Y, accraw.AXIS_Z);

		sprintf(disp,"GYRO X: %6.2f", data[0]);
		BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"GYRO Y: %6.2f", data[1]);
		BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"GYRO Z: %6.2f", data[2]);
		BSP_LCD_DisplayStringAt(0, 50, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"SAMPLES: %d           ", count);
		BSP_LCD_DisplayStringAt(0, 70, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"QUEUE: %lu           ", uxQueueMessagesWaiting(hGyroQueue));
		BSP_LCD_DisplayStringAt(0, 90, (uint8_t*)disp, LEFT_MODE);
		led1.toggle();

//		count = gyro.GetFifoSourceReg() & 0x1F;
//		for (uint8_t i = 0; i < count; i++)
//			gyro.GetAngRateRaw(&raw);

		//vTaskDelay(150 / portTICK_RATE_MS);
//		vTaskSuspend(NULL);
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {

		}

	}
}


int main(int argc, char* argv[])
{
	char buffer[512];
	uint32_t freq = HAL_RCC_GetSysClockFreq();

	button.callback(&led2, &DigitalOut::toggle);

	trace_printf("Starting main_task:, CPU freq: %d, f=%f\n", freq, 0.75);

	  /* Create tasks */
	xTaskCreate(
		main_task, /* Function pointer */
		"Task1", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		&hMain /* Task handle */
		);

	xTaskCreate(
		secondary_task, /* Function pointer */
		"Task2", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);

	xTaskCreate(
		secondary_task, /* Function pointer */
		"Task3", /* Task name - for debugging only*/
		configMINIMAL_STACK_SIZE, /* Stack depth in words */
		(void*) NULL, /* Pointer to tasks arguments (parameter) */
		tskIDLE_PRIORITY + 2UL, /* Task priority*/
		NULL /* Task handle */
		);

	vTaskList(buffer);
	trace_printf("Tasks: \n%s\n\n", buffer);

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

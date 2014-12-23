//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------


#include <stdint.h>
#include <stdio.h>
#include <stdexcept>

#include "../application/digitalin.h"
#include "../application/digitalout.h"
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f429i_discovery_gyroscope.h"
#include "stm32f429i_discovery_lcd.h"

void* __dso_handle = 0;

DigitalOut led1(USER_LED1);
DigitalOut led2(USER_LED2);
DigitalIn button(USER_BUTTON, DigitalIn::PullDown, DigitalIn::InterruptRising);

extern "C" void EXTI0_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(0);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
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
	init_lcd();

	float data[3];
	BSP_GYRO_Init();


	// Infinite loop
	char disp[128] = {0};
	while (1) {
		BSP_GYRO_GetXYZ(data);
		data[0] = data[0] / 250.0f;
		data[1] = data[1] / 250.0f;
		data[2] = data[2] / 250.0f;

		trace_printf("GYRO ID: 0x%x, data: %f, %f, %f\n", BSP_GYRO_ReadID(), data[0], data[1], data[2]);

		sprintf(disp,"GYRO X: %3.4f", data[0]);
		BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"GYRO Y: %3.4f", data[1]);
		BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"GYRO Z: %3.4f", data[2]);
		BSP_LCD_DisplayStringAt(0, 50, (uint8_t*)disp, LEFT_MODE);

		vTaskDelay(250 / portTICK_RATE_MS);
		try {
			throw (std::runtime_error("timeout"));
		} catch (std::exception& e) {
//			trace_printf("main_task:, exception: %s, f=%f\n", e.what(), 0.75);
		}
		led1.toggle();
	}
}


int main(int argc, char* argv[])
{
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
		NULL /* Task handle */
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

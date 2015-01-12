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
#include "l3gd20.h"
#include "lsm303d.h"
#include "matrix.h"
#include "attitudetracker.h"

void* __dso_handle = 0;

DigitalOut led1(USER_LED1);
DigitalOut led2(USER_LED2);
DigitalIn gyro_int2(PA_2, DigitalIn::PullUp, DigitalIn::InterruptRising);
DigitalIn acc_int2(PB_4, DigitalIn::PullUp, DigitalIn::InterruptRising);
DigitalIn button(USER_BUTTON, DigitalIn::PullDown, DigitalIn::InterruptRising);
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
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(2);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

extern "C" void EXTI4_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(4);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

void gyro_isr()
{
	if (gyro_int2 || acc_int2) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t msg = 1;
		xQueueSendFromISR(hGyroQueue, &msg, &xHigherPriorityTaskWoken);
	}
}


void acc_isr()
{
	if (gyro_int2 || acc_int2) {
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

	vTaskDelay(200 / portTICK_RATE_MS);

	BSP_LCD_Clear(LCD_COLOR_WHITE);
}

void main_task(void *pvParameters)
{
	SPIMaster spi4(SPI4, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{PE_2, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPIx_SCK_PIN */
				{PE_5, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPIx_MISO_PIN */
				{PE_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},			/* DISCOVERY_SPIx_MOSI_PIN */
			}, {
				{PG_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* ACCEL_CS_PIN */
			});


	SPIMaster spi5(SPI5, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{PF_7, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_SCK_PIN */
				{PF_8, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_MISO_PIN */
				{PF_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},			/* DISCOVERY_SPIx_MOSI_PIN */
			}, {
				{PC_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* GYRO_CS_PIN */
				{PG_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* ACCEL_CS_PIN */
			});
	L3GD20 gyro(spi5, 0);
	LSM303D accel(spi4, 0);
	uint8_t gyr_wtm = 18;
	uint8_t acc_wtm = 17;
	L3GD20::AxesDPS_t gyr_axes;
	LSM303D::AxesAcc_t acc_axes;
	QuaternionD q;
	attitudetracker att;

	gyro_int2.callback(gyro_isr);
	acc_int2.callback(acc_isr);
	init_lcd();
	hGyroQueue = xQueueCreate(10, sizeof(uint32_t));

	gyro.SetMode(L3GD20::NORMAL);
	gyro.SetODR(L3GD20::ODR_190Hz_BW_70);
	gyro.SetFullScale(L3GD20::FULLSCALE_500);
	gyro.SetBDU(L3GD20::MEMS_ENABLE);
	gyro.SetWaterMark(gyr_wtm);
	gyro.FIFOModeEnable(L3GD20::FIFO_STREAM_MODE);
	gyro.SetInt2Pin(0);
	gyro.SetInt2Pin(L3GD20_WTM_ON_INT2_ENABLE| L3GD20_OVERRUN_ON_INT2_ENABLE);
	gyro.SetAxis(L3GD20_X_ENABLE|L3GD20_Y_ENABLE|L3GD20_Z_ENABLE);

	accel.SetODR(LSM303D::ODR_200Hz);
	accel.SetFullScale(LSM303D::FULLSCALE_8);
	accel.SetAxis(LSM303D::X_ENABLE | LSM303D::Y_ENABLE | LSM303D::Z_ENABLE);
	accel.FIFOModeSet(LSM303D::FIFO_STREAM_MODE);
	accel.SetThreshold(acc_wtm);
	accel.SetInt2Pin(LSM303D_INT2_OVERRUN_ENABLE|LSM303D_INT2_FTH_ENABLE);
	accel.SetInt2Pin(0);

	// Infinite loop
	TickType_t ticks = 0, oldticks = 0;
	Vector3d gyr_bias;
	char disp[128] = {0};

	sprintf(disp,"Calibrating Gyro...");
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)disp, LEFT_MODE);

	for (int i = 0; i < 50; i++) {
		gyro.GetFifoAngRateDPS(&gyr_axes);
		gyr_bias.at(0) += gyr_axes.AXIS_X;
		gyr_bias.at(1) += gyr_axes.AXIS_Y;
		gyr_bias.at(2) += gyr_axes.AXIS_Z;
		led1.toggle();
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {
		}
	}
	gyr_bias = gyr_bias / 50.0;
	while (1) {
		uint8_t gyr_samples = gyro.GetFifoSourceReg() & 0x1F;
		uint8_t acc_samples = accel.GetFifoSourceFSS();
		if (gyr_samples >= gyr_wtm)
			gyro.GetFifoAngRateDPS(&gyr_axes);
		if (acc_samples > acc_wtm)
			accel.GetFifoAcc(&acc_axes);
		Vector3d gyr_data = Vector3d(gyr_axes.AXIS_X, gyr_axes.AXIS_Y, gyr_axes.AXIS_Z) - gyr_bias;
//		trace_printf("Accelerometer ID: 0x%x, g: %3.2f %3.2f %3.2f\n", accel.ReadReg8(LSM303D_WHO_AM_I), g.AXIS_X, g.AXIS_Y, g.AXIS_Z);
		ticks = xTaskGetTickCount() - oldticks;
		oldticks = xTaskGetTickCount();
		att.track_gyroscope(DEG2RAD(gyr_data), ticks * portTICK_PERIOD_MS / (float)1000.0);
		q = att.get_attitude();

		sprintf(disp,"GYRO X: %6.2f         ", gyr_data.at(0));
		BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"GYRO Y: %6.2f         ", gyr_data.at(1));
		BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"GYRO Z: %6.2f         ", gyr_data.at(2));
		BSP_LCD_DisplayStringAt(0, 50, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"SAMPLES: %d           ", gyr_samples);
		BSP_LCD_DisplayStringAt(0, 70, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"QUEUE: %lu            ", uxQueueMessagesWaiting(hGyroQueue));
//		BSP_LCD_DisplayStringAt(0, 90, (uint8_t*)disp, LEFT_MODE);

		sprintf(disp,"ACCL X: %6.2f", acc_axes.AXIS_X);
		BSP_LCD_DisplayStringAt(0, 110, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"ACCL Y: %6.2f", acc_axes.AXIS_Y);
		BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"ACCL Z: %6.2f", acc_axes.AXIS_Z);
		BSP_LCD_DisplayStringAt(0, 150, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"SAMPLES: %d           ", acc_samples);
		BSP_LCD_DisplayStringAt(0, 170, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"Attitude:             ");
		BSP_LCD_DisplayStringAt(0, 190, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"W:      %6.2f              ", q.w);
		BSP_LCD_DisplayStringAt(0, 210, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"X:      %6.2f              ", q.x);
		BSP_LCD_DisplayStringAt(0, 230, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"Y:      %6.2f              ", q.y);
		BSP_LCD_DisplayStringAt(0, 250, (uint8_t*)disp, LEFT_MODE);
		sprintf(disp,"Z:      %6.2f              ", q.z);
		BSP_LCD_DisplayStringAt(0, 270, (uint8_t*)disp, LEFT_MODE);

		sprintf(disp,"UPDATE: %d ms          ", (int)(ticks * portTICK_PERIOD_MS));
		BSP_LCD_DisplayStringAt(0, 290, (uint8_t*)disp, LEFT_MODE);
		led1.toggle();

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

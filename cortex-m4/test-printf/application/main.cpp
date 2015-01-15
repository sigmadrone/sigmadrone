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

extern "C" void SPI4_IRQHandler(void)
{
	SPISlave::vector_handler(4);
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


void spi_slave_task(void *pvParameters)
{
	trace_printf("SPI Slave task...\n");
	SPISlave spi4(SPI4, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{PE_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_SCK_PIN */
				{PE_4, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_NSS_PIN */
				{PE_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_MISO_PIN */
				{PE_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI4},		/* DISCOVERY_SPI4_MOSI_PIN */
			}, {
			});


	spi4.Start();
	while (1) {

	}
	char tx = 't', rx = 0;
	SPI_HandleTypeDef SpiHandle;
	/*##-1- Configure the SPI peripheral #######################################*/
	/* Set the SPI parameters */
	SpiHandle.Instance               = SPI4;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_HARD_INPUT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
	SpiHandle.Init.Mode              = SPI_MODE_SLAVE;
	if(HAL_SPI_Init(&SpiHandle) != HAL_OK) {
		trace_printf("SPI Slave task... error\n");
	}

	while (1) {
		switch (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &tx, (uint8_t *) &rx, sizeof(tx), 10000)) {
		case HAL_OK:
			trace_printf("SPI Slave received: %c\n", rx);
			break;

		case HAL_TIMEOUT:
			trace_printf("SPI Slave TX/RX timed out\n");
			break;

			/* An Error occurred______________________________________________________*/
		case HAL_ERROR:
			trace_printf("SPI Slave TX/RX error\n");
			break;

		default:
			break;
		}
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

	vTaskDelay(2000 / portTICK_RATE_MS);

	BSP_LCD_Clear(LCD_COLOR_WHITE);
}

void main_task(void *pvParameters)
{
	SPIMaster spi5(SPI5, SPI_BAUDRATEPRESCALER_16, 0x2000, {
				{PF_7, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_SCK_PIN */
				{PF_8, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},		/* DISCOVERY_SPIx_MISO_PIN */
				{PF_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF5_SPI5},			/* DISCOVERY_SPIx_MOSI_PIN */
			}, {
				{PC_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* GYRO_CS_PIN */
				{PG_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* ACCEL_CS_PIN */
				{PG_3, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_MEDIUM, 0},				/* SLAVE_CS_PIN */
			});
	L3GD20 gyro(spi5, 0);
	LSM303D accel(spi5, 1);
	uint8_t gyr_wtm = 20;
	uint8_t acc_wtm = 17;
	uint8_t bias_iterations = 40;
	L3GD20::AxesDPS_t gyr_axes;
	LSM303D::AxesAcc_t acc_axes;
	QuaternionD q;
	attitudetracker att;

	gyro_int2.callback(gyro_isr);
	acc_int2.callback(acc_isr);
	init_lcd();
	hGyroQueue = xQueueCreate(10, sizeof(uint32_t));

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
//	spi5.read(2, (uint8_t*)disp, 1);
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
	TickType_t displayUpdateTicks = xTaskGetTickCount();
	while (1) {
		uint32_t msg;
		if( xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * 5000 ) ) {
		}
		uint8_t gyr_samples = gyro.GetFifoSourceReg() & 0x1F;
		uint8_t acc_samples = accel.GetFifoSourceFSS();
		if (gyr_samples >= gyr_wtm)
			gyro.GetFifoAngRateDPS(&gyr_axes);
		if (acc_samples > acc_wtm)
			accel.GetFifoAcc(&acc_axes);
		Vector3d gyr_data = Vector3d(gyr_axes.AXIS_X, gyr_axes.AXIS_Y, gyr_axes.AXIS_Z) - gyr_bias;
		ticks = xTaskGetTickCount() - oldticks;
		oldticks = xTaskGetTickCount();
		att.track_gyroscope(DEG2RAD(gyr_data), ticks * portTICK_PERIOD_MS / (float)1000.0);
		q = att.get_attitude();

		if ((oldticks - displayUpdateTicks) * portTICK_PERIOD_MS > 200) {
			displayUpdateTicks = oldticks;
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
		}
		led1.toggle();
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
		spi_slave_task, /* Function pointer */
		"SPI Slave Task", /* Task name - for debugging only*/
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
		trace_printf("Hello world, freq: %d\n", freq);
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

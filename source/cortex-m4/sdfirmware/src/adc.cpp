/*
 * adc.cpp
 *
 *  Created on: Sep 8, 2015
 *      Author: svassilev
 */

#include "adc.h"
#include <string.h>
#include <assert.h>

ADCHandle* ADCHandle::adc_handles_[3] = {0};

ADCHandle::ADCHandle(
		ADC_TypeDef* adc_instance,
		uint32_t adc_channel,
		PinName pin) : gpio_(pin, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_MEDIUM, 0)
{
	init(adc_instance, adc_channel);
}

ADCHandle::ADCHandle(
		ADC_TypeDef* adc_instance,
		uint32_t adc_channel,
		const GPIOPin& gpio) : gpio_(gpio)
{
	init(adc_instance, adc_channel);
}

uint32_t ADCHandle::read_value()
{
	HAL_ADC_Start(&adc_handle_);
	if (HAL_ADC_PollForConversion(&adc_handle_, 50) != HAL_OK) {
		return INVALID_CONV_VALUE;
	}
	HAL_ADC_Stop(&adc_handle_);
	return HAL_ADC_GetValue(&adc_handle_);
}

uint32_t ADCHandle::max_value()
{
	return 1 << 12;
}

void ADCHandle::init(ADC_TypeDef* adc_instance, uint32_t adc_channel)
{
	set_adc_instance(adc_instance);

	memset(&adc_handle_,0,sizeof(adc_handle_));
	adc_handle_.Instance          = adc_instance;

	adc_handle_.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
	adc_handle_.Init.Resolution = ADC_RESOLUTION12b;
	adc_handle_.Init.ScanConvMode = DISABLE;
	adc_handle_.Init.ContinuousConvMode = ENABLE;
	adc_handle_.Init.DiscontinuousConvMode = DISABLE;
	adc_handle_.Init.NbrOfDiscConversion = 0;
	adc_handle_.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adc_handle_.Init.ExternalTrigConv = DISABLE;//ADC_EXTERNALTRIGCONV_T1_CC1;
	adc_handle_.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle_.Init.NbrOfConversion = 1;
	adc_handle_.Init.DMAContinuousRequests = DISABLE;//ENABLE;
	adc_handle_.Init.EOCSelection = ENABLE;
	if (HAL_ADC_Init(&adc_handle_) != HAL_OK) {
		throw std::runtime_error("HAL_ADC_Init failed");
	}

	ADC_ChannelConfTypeDef chConfig;
	chConfig.Channel = adc_channel;
	chConfig.Rank = 1;
	chConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	chConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&adc_handle_, &chConfig) != HAL_OK) {
		throw std::runtime_error("HAL_ADC_ConfigChannel failed");
	}

	if (HAL_ADC_Start(&adc_handle_) != HAL_OK) {
		throw std::runtime_error("HAL_ADC_Start failed");
	}
}

ADCHandle* ADCHandle::from_adc_instance(ADC_TypeDef* instance)
{
	return adc_handles_[adc_instance_to_slot(instance)];
}

void ADCHandle::set_adc_instance(ADC_TypeDef* instance)
{
	if (from_adc_instance(instance)) {
		throw std::runtime_error("ADC handle already initialized!");
	}
	adc_handles_[adc_instance_to_slot(instance)] = this;
}

void ADCHandle::clear_adc_instance(ADC_TypeDef* instance)
{
	adc_handles_[adc_instance_to_slot(instance)] = 0;
}

uint32_t ADCHandle::adc_instance_to_slot(ADC_TypeDef* instance)
{
	if (ADC1 == instance) {
		return 0;
	}
	if (ADC2 == instance) {
		return 1;
	}
	if (ADC3 == instance) {
		return 2;
	}
	throw std::invalid_argument("Invalid ADC instance passed");
}

ADCHandle::~ADCHandle()
{
	disable_clock();
}

void ADCHandle::enable_clock()
{
	switch (adc_instance_to_slot(adc_handle_.Instance)) {
	case 0:
		__ADC1_CLK_ENABLE();
		break;
	case 1:
		__ADC2_CLK_ENABLE();
		break;
	case 2:
		__ADC3_CLK_ENABLE();
		break;
	default:
		assert(false); // should never get here
	}
}

void ADCHandle::disable_clock()
{
	switch (adc_instance_to_slot(adc_handle_.Instance)) {
	case 0:
		__ADC1_CLK_DISABLE();
		break;
	case 1:
		__ADC2_CLK_DISABLE();
		break;
	case 2:
		__ADC3_CLK_DISABLE();
		break;
	default:
		assert(false); // should never get here
	}
}


void ADCHandle::msp_init()
{
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO clock */
	// ADCx_CHANNEL_GPIO_CLK_ENABLE();
	gpio_.init();

	/* ADCx Periph clock enable */
	enable_clock();


#if 0
  // DMAx_CLK_ENABLE();


  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured */
  hdma_adc.Instance = ADCx_DMA_STREAM;

  hdma_adc.Init.Channel  = ADCx_DMA_CHANNEL;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

  HAL_DMA_Init(&hdma_adc);

  /* Associate the initialized DMA handle to the the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
#endif
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	ADCHandle::from_adc_instance(hadc->Instance)->msp_init();
}

#if 0
/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
#if 0
  static DMA_HandleTypeDef  hdma_adc;

  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC3 Channel8 GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);

  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associate to transmission process */
  HAL_DMA_DeInit(&hdma_adc);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);
#endif
}
#endif

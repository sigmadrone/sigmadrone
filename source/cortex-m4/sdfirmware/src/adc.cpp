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
		PinName pin,
		float v_ref) :
				gpio_(pin, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_MEDIUM, 0),
				v_ref_(v_ref)
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

float ADCHandle::read_value_as_voltage()
{
	uint32_t val = read_value();
	return (val != INVALID_CONV_VALUE) ? ((float)val / (float)max_value() * v_ref()) : 0;
}

uint32_t ADCHandle::max_value()
{
	return 1 << 12;
}

float ADCHandle::v_ref()
{
	return v_ref_;
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
	/* Enable GPIO clock */
	gpio_.init();

	/* ADCx Periph clock enable */
	enable_clock();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	ADCHandle::from_adc_instance(hadc->Instance)->msp_init();
}

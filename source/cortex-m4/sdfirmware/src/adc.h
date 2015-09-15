/*
 * adc.h
 *
 *  Created on: Sep 8, 2015
 *      Author: svassilev
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <vector>
#include <stdexcept>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f429xx.h"
#include "gpiopin.h"
#include "units.h"

/*
 * Simple ADC class that supports only one regular channel, with data transfered over DMA
 */
class ADCHandle {
public:
	static const uint32_t INVALID_CONV_VALUE = 0xffffffff;

	/** Constructs and initializes a timer instance
	 *  @param adc Pointert to the internal ADC_TypeDef struct to be used, one of ADC1, ADC2 or ADC3
	 *  @param adc_channel ADC channel to be used, one of ADC_CHANNEL_0 .. ADC_CHANNEL_16
	 *  @param v_ref Reference voltage
	 */
	ADCHandle(ADC_TypeDef* adc, uint32_t adc_channel, PinName pin, float v_ref = 3.3);
	~ADCHandle();
	uint32_t read_value();
	uint32_t max_value();
	float v_ref();
	float read_value_as_voltage();

private:
	void init(ADC_TypeDef* adc, uint32_t adc_channel);
	void msp_init();
	void set_adc_instance(ADC_TypeDef* instance);
	void enable_clock();
	void disable_clock();
	static ADCHandle* from_adc_instance(ADC_TypeDef* instance);
	static void clear_adc_instance(ADC_TypeDef* instance);
	static uint32_t adc_instance_to_slot(ADC_TypeDef* instance);

	friend void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);

private:
	ADC_HandleTypeDef adc_handle_;
	GPIOPin gpio_;
	float v_ref_;

	static ADCHandle* adc_handles_[3];
};

namespace colibri {

class Voltmeter {
public:
	static constexpr float r1_div_ = 100.0f;
	static constexpr float r2_div_ = 21.5f;
	Voltmeter() : adc_(ADC1, ADC_CHANNEL_9, PB_1) {}
	Voltage measure()
	{
		return Voltage::from_volts(adc_.read_value_as_voltage() * (r1_div_+r2_div_) / r2_div_);
	}
private:
	ADCHandle adc_;
};

};

#endif /* ADC_H_ */

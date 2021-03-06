/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <vector>
#include <stdexcept>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f429xx.h"
#include "colibrihwmap.h"
#include "units.h"
#include "digitalout.h"
#include "FreeRTOS.h"
#include "task.h"

/*
 * Simple ADC class that supports only one regular channel, with data transfered over DMA
 */
class ADCHandle {
public:
	static const uint32_t INVALID_CONV_VALUE = 0xffffffff;

	/** Constructs and initializes an ADC handle instance
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
	Voltmeter() : adc_(BATTERY_MONITOR_ADC, BATTERY_MONITOR_ADC_CHANNEL, BATTERY_MONITOR_PIN, 3.25),
			voltmeter_ctrl_(BATTERY_MONITOR_ONOFF_PIN, DigitalOut::OutputDefault, DigitalOut::PullDefault, 0) {}
	Voltage measure()
	{
		voltmeter_ctrl_.write(1);
		float voltage;
		vTaskDelay(300 / portTICK_RATE_MS);
		for (uint32_t i = 0; i < 8 && (voltage = adc_.read_value_as_voltage()) < 0.1; ++i) {
			vTaskDelay(300 / portTICK_RATE_MS);
		}
		voltmeter_ctrl_.write(0);
		return Voltage::from_volts(voltage * (r1_div_+r2_div_) / r2_div_);
	}
private:
	ADCHandle adc_;
	DigitalOut voltmeter_ctrl_;
};

};

#endif /* ADC_H_ */

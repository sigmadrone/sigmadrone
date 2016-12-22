#include <stm32f4xx_hal.h>

#include <iostream>

#include "poweroff.h"
#include "digitalin.h"
#include "digitalout.h"

PowerOff::PowerOff(PinName pwrint_name, PinName request_name, PinName ready_name, PinName pwrkill_name, const std::vector<PinName>& led_names)
	: pwrint_(pwrint_name, DigitalIn::PullNone, DigitalIn::InterruptFalling)
	, request_(request_name, DigitalOut::OutputDefault, DigitalOut::PullDefault, 0)
	, ready_(ready_name, DigitalIn::PullNone, DigitalIn::InterruptFalling)
	, pwrkill_(pwrkill_name, DigitalOut::OutputDefault, DigitalOut::PullDefault, 1)
{
	for (auto name : led_names)
		leds_.push_back(DigitalOut(name));
	pwrint_.callback(this, &PowerOff::initiate);
	ready_.callback(this, &PowerOff::shutdown);
}

PowerOff::~PowerOff()
{

}

void PowerOff::leds_turn_on()
{
	for (auto led : leds_)
		led.write(1);
}

void PowerOff::leds_turn_off()
{
	for (auto led : leds_)
		led.write(0);
}

void PowerOff::leds_toggle()
{
	for (auto led : leds_)
		led.toggle();
}


void PowerOff::delay()
{
	for (unsigned int i = 0; i < wait_iterations; i++)
		;
}

void PowerOff::initiate()
{
	leds_turn_off();
	std::cout << "  -- PowerOff initiated... ready pin is: " << ready_.read() << std::endl;
	for (unsigned long i = 0; i < poweroff_iterations; i++) {
		leds_[i % 3].toggle();
		request_.write(1);
		delay();
		request_.write(0);
		if (ready_.read() == 0)
			break;
	}
	pwrkill_.write(0);
}

void PowerOff::shutdown()
{
	pwrkill_.write(0);
}


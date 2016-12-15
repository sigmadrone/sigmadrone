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

#ifndef _POWEROFF_H_
#define _POWEROFF_H_

#include <vector>
#include "digitalin.h"
#include "digitalout.h"


class PowerOff {
public:
	PowerOff(PinName pwrint_name, PinName request_name, PinName ready_name, PinName pwrkill_name, const std::vector<PinName>& led_names = {});
	~PowerOff();

protected:
	static constexpr unsigned int wait_iterations = 2000000;
	static constexpr unsigned int poweroff_iterations = 150;
	void initiate();
	void shutdown();
	void leds_turn_on();
	void leds_turn_off();
	void leds_toggle();
	void delay();

protected:
	DigitalIn pwrint_;
	DigitalOut request_;
	DigitalIn ready_;
	DigitalOut pwrkill_;
	std::vector<DigitalOut> leds_;
};

#endif

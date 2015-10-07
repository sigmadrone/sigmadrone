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
#ifndef SERVOMOTOR_H_
#define SERVOMOTOR_H_

class servomotor
{
protected:
	bool valid_;				/* Servo motor good state */
	double pulse_;				/* Servo motor current position pulse length in msec */
	double reset_;				/* Servo motor reset pulse length in mSec */
	double neutral_;			/* Servo motor neutral position pulse length in mSec */
	double negative_;			/* Servo motor max negative position pulse length in mSec */
	double positive_;			/* Servo motor max positive position pulse length in mSec */

public:
	servomotor();
	servomotor(double reset, double neutral, double negative, double positive);

	/**
	 * Return the current offset
	 */
	double offset();
	/**
	 * Set the motor position offset from neutral.
	 * @percent offset from -1.0 to +1.0
	 */
	void offset(double percent);

	/**
	 * Set the motor position offset from neutral.
	 * @percent offset will be clipped within min to max range.
	 */
	void offset_clamp(double percent, double min, double max);

	/**
	 * Reset the motor.
	 */
	void reset();

	/**
	 * Return the current pulse in mSec, required for the motor to maintain the current offset
	 */
	double pulse() const;

	/**
	 * Check if the class is properly initialized and ready.
	 */
	bool valid() const;
};

#endif

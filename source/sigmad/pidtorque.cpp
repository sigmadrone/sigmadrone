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
#include <stdio.h>
#include "pidtorque.h"

pidtorque::pidtorque() :
	target_q_(1), old_q_(1)
{
	reset(0.0, 0.0, 0.0);
}

pidtorque::~pidtorque()
{

}

void pidtorque::reset(double Kp, double Ki, double Kd)
{
	Kp_ = Kp;
	Ki_ = Ki;
	Kd_ = Kd;
	integral_error_ = Vector3d();
	error_ = Vector3d();
}

void pidtorque::set_target(const QuaternionD &setQ)
{
	target_q_ = setQ;
}

Vector3d pidtorque::get_xy_torque(const QuaternionD &inQ, const Vector3d& Omega, double dT)
{
	Vector3d torq;
	Vector3d differentialError;
	Vector3d Zset = target_q_.rotate(Vector3d(0.0, 0.0, 1.0));
	Vector3d Zin = (~inQ).rotate(Vector3d(0.0, 0.0, 1.0));
	QuaternionD Qtorq = QuaternionD::fromVectors(Zin, Zset);
	Vector3d error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;
	double Kp = 1000.0 * Kp_ / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	double Ki = 1000.0 * Ki_ / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	double Kd = 1000.0 * Kd_ / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	double err = error.length();

	integral_error_ = integral_error_ + error * dT;
	integral_error_ = integral_error_ - integral_error_ * 0.000001;
	if (integral_error_.length() * Ki > 30.0/1000.0)
		integral_error_ = integral_error_ * 0.85;
	differentialError = (error - error_)/dT;
	torq = error.normalize() * pow(err, 1.00) * Kp +
			differentialError * Kd +
			integral_error_.normalize() * pow(integral_error_.length(), 1.3) * Ki * 0.0;
	error_ = error;
	return torq;
}

Vector3d pidtorque::get_z_torque(const QuaternionD &inQ, const Vector3d& Omega, double dT)
{
	Vector3d torq;
	Vector3d differentialError;
	Vector3d Zset = target_q_.rotate(Vector3d(0.0, 0.0, 1.0));
	Vector3d Zin = (~inQ).rotate(Vector3d(0.0, 0.0, 1.0));
	QuaternionD Qtorq = QuaternionD::fromVectors(Zin, Zset);
	Vector3d error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;
	double Kp = 50.0 / 1000.0  * Kp_ * 22.5 / 100.0 / (3.1415/2);
	double Ki = 50.0 / 1000.0  * Ki_ * 22.5 / 100.0 / (3.1415/2);
	double Kd = 50.0 / 1000.0  * Kd_ * 22.5 / 100.0 / (3.1415/2);
	double err = error.length();

	integral_error_ = integral_error_ + error * dT;
	integral_error_ = integral_error_ - integral_error_ * 0.000001;
	if (integral_error_.length() * Ki > 30.0/1000.0)
		integral_error_ = integral_error_ * 0.85;
	if (dT > 0.0001)
		differentialError = (error - error_)/dT;
	torq = error.normalize() * pow(err, 1.00) * Kp +
			differentialError * Kd +
			integral_error_.normalize() * pow(integral_error_.length(), 1.3) * Ki * 0.0;
	error_ = error;
	return torq;
}

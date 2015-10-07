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

#ifndef ATTCONTROLLER_H_
#define ATTCONTROLLER_H_

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include "pidtorque.h"
#include "matrix.h"

class server_app;

class attcontroller {
public:
	attcontroller(server_app& app);
	virtual ~attcontroller();
	void start();
	void stop();
	bool is_running();
	QuaternionD get_attitude() const;
	void set_thrust_dir(const Vector3d& thrustdir);
	void set_correction_thrust(const Vector4d& ct);
	Vector4d get_correction_thrust();

protected:
	void worker();

protected:
	server_app& app_;
	bool exit_;
	boost::shared_ptr<boost::thread> thread_;
	QuaternionD attQ_;
	QuaternionD targetQ_;

public:
	Vector3d M0_;
	Vector3d M1_;
	Vector3d M2_;
	Vector3d M3_;

	/* Motor positions */
	Vector3d P0_;
	Vector3d P1_;
	Vector3d P2_;
	Vector3d P3_;

	/* Motor rotation direction and the torque generated around Z axis
	 * resulting from that rotation.
	 */
	Vector4d rot_;
	Vector3d thrustdir_;
	Vector4d ct_;
	pidtorque pid_;
	double thrust_;

};


#endif /* ATTCONTROLLER_H_ */

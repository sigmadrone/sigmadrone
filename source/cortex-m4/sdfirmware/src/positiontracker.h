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

#ifndef POSITIONTRACKER_H_
#define POSITIONTRACKER_H_

#include "d3math.h"

class PositionTracker {
public:
	PositionTracker(const Vector3f& earth_g, float vleak, int calibration_samples = 2000);
	void acceleration(Vector3f a, float dt);
	void calibration(Vector3f a, float dt);
	Vector3f velocity() const;
	Vector3f position() const;

protected:
	float vleak_;
	int calibration_samples_;
	int current_sample_;
	Vector3f earth_g_;
	Vector3f velocity_;
	Vector3f position_;
	Vector3f vbias_;
};

#endif

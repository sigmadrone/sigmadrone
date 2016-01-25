#include "positiontracker.h"

PositionTracker::PositionTracker(const Vector3f& earth_g, float vleak, int calibration_samples)
	: vleak_(vleak)
	, calibration_samples_(calibration_samples)
	, current_sample_(-100)
	, earth_g_(earth_g)
{

}

void PositionTracker::acceleration(Vector3f a, float dt)
{
	if (current_sample_ < 0) {
		current_sample_++;
		return;
	}

	a -= earth_g_;
	Vector3f dV = a * dt;
	Vector3f dP = (velocity() * 2 + dV) * dt / 2.0;
	velocity_ += dV;
	if (current_sample_ < calibration_samples_) {
		current_sample_++;
		vbias_ += velocity_;
		velocity_ *= vleak_;
		return;
	}
	position_ += dP;
	position_ *= vleak_;
	velocity_ *= vleak_;
}


Vector3f PositionTracker::velocity() const
{
	if (current_sample_ < calibration_samples_)
		return Vector3f();
	return velocity_ - (vbias_ / calibration_samples_);
}

Vector3f PositionTracker::position() const
{
	return position_;
}


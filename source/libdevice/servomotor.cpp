#include <stdexcept>
#include <sstream>
#include "servomotor.h"

/*
 * Default constructor is provided to allow the
 * creation of array of motors. The valid_ state is
 * set to false, so the motor is not usable in this state.
 */
servomotor::servomotor()
	: valid_(false)
	, pulse_(0.0)
	, reset_(0.0)
	, neutral_(0.0)
	, negative_(0.0)
	, positive_(0.0)
{
}

servomotor::servomotor(double reset, double neutral, double negative, double positive)
	: valid_(true)
	, pulse_(reset)
	, reset_(reset)
	, neutral_(neutral)
	, negative_(negative)
	, positive_(positive)
{
	if (reset < 0 || neutral < 0 || negative < 0 || positive < 0)
		std::range_error("servomotor::servomotor initialization failed.");
}

double servomotor::offset()
{
	if (pulse_ < neutral_)
		return (neutral_ - pulse_) / (neutral_ - negative_);
	else
		return (pulse_ - neutral_) / (positive_ - neutral_);
}

void servomotor::offset(double percent)
{
	if (percent > 1.01 || percent < -1.01) {
		std::stringstream strpercent;
		strpercent << percent;
		throw std::invalid_argument("servomotor::offset, invalid parameter: " + strpercent.str());
	}
	if (percent < 0.0)
		pulse_ = neutral_ - (neutral_ - negative_) * percent;
	else
		pulse_ = neutral_ + (positive_ - neutral_) * percent;
}

void servomotor::reset()
{
	pulse_ = reset_;
}

double servomotor::pulse() const
{
	if (!valid_)
		throw std::runtime_error("servomotor::pulse must not be called until the motor is properly initialized.");
	return pulse_;
}

bool servomotor::valid() const
{
	return valid_;
}

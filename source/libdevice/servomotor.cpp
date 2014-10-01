#include "stdexcept"
#include "sstream"
#include "servomotor.h"

servomotor::servomotor()
	: pulse_(0.0)
	, reset_(0.0)
	, neutral_(0.0)
	, negative_(0.0)
	, positive_(0.0)
{
}

servomotor::servomotor(double reset, double neutral, double negative, double positive)
	: pulse_(reset)
	, reset_(reset)
	, neutral_(neutral)
	, negative_(negative)
	, positive_(positive)
{

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
	return pulse_;
}

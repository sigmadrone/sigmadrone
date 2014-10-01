#ifndef SERVOMOTOR_H_
#define SERVOMOTOR_H_

class servomotor
{
protected:
	double pulse_;				/* Servo motor current position pulse length in msec */

public:
	const double reset_;		/* Servo motor reset pulse length in mSec */
	const double neutral_;		/* Servo motor neutral position pulse length in mSec */
	const double negative_;		/* Servo motor max negative position pulse length in mSec */
	const double positive_;		/* Servo motor max positive position pulse length in mSec */

	servomotor();
	servomotor(double reset, double neutral, double negative, double positive);

	/**
	 * Set the motor position offset from neutral.
	 * @percent offset from -1.0 to +1.0
	 */
	void offset(double percent);

	/**
	 * Reset the motor.
	 */
	void reset();

	/**
	 * Return the current pulse in mSec, required for the motor to maintain the current offset
	 */
	double pulse() const;
};

#endif

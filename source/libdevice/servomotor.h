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

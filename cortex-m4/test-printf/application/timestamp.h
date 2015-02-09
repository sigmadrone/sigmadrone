/*
 * timestamp.h
 *
 *  Created on: Feb 3, 2015
 *      Author: svetlio
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "hwtimer.h"

class TimeStamp {
public:
	static bool Init(HwTimer::Id timer_id = HwTimer::TIMER_7);
	static void Cleanup();

	TimeStamp();
	~TimeStamp();
	void time_stamp();
	TimeSpan elapsed();
private:
	struct Stamp {
		Stamp() : current_elapsed_count_(0) {}
		Stamp(const TimeSpan& period, uint64_t elapsed_count) : current_period_(period),
				current_elapsed_count_(elapsed_count) {}
		TimeSpan current_period_;
		uint64_t current_elapsed_count_;
	};
	static Stamp read_timer();

private:
	static HwTimer* timer_;
	Stamp stamp_;
	Stamp last_read_stamp_;
};

#endif /* TIMESTAMP_H_ */

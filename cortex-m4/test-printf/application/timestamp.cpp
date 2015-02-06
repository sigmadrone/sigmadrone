/*
 * timestamp.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: svetlio
 */

#include "timestamp.h"

HwTimer* TimeStamp::timer_ = 0;

TimeStamp::TimeStamp() : stamp_()
{
	time_stamp();
}

TimeStamp::~TimeStamp() {}

void TimeStamp::time_stamp()
{
	if (!timer_) {
		return;
	}
	stamp_ = read_timer();
}

TimeStamp::Stamp TimeStamp::read_timer()
{
	TimeSpan ts;
	volatile uint64_t elapsed_count;
	volatile uint64_t elapsed_count_2;
	do {
		elapsed_count = timer_->period_elapsed_count();
		ts = timer_->time_elapsed();
	} while ((elapsed_count_2 = timer_->period_elapsed_count()) != elapsed_count);
	return Stamp(ts, elapsed_count);
}

TimeSpan TimeStamp::elapsed()
{
	TimeSpan ts_elapsed;
	Stamp now = read_timer();
	if (now.current_elapsed_count_ == stamp_.current_elapsed_count_) {
		ts_elapsed = now.current_period_ - stamp_.current_period_;
	} else {
		assert(now.current_elapsed_count_ > stamp_.current_elapsed_count_);
		ts_elapsed = timer_->period() - stamp_.current_period_ + now.current_period_;
		ts_elapsed += timer_->period() * (now.current_elapsed_count_ - stamp_.current_elapsed_count_ - 1);
	}
	return ts_elapsed;
}

bool TimeStamp::Init(HwTimer::Id timer_id)
{
	assert(timer_id != HwTimer::TIMER_1 &&
			timer_id != HwTimer::TIMER_8);
	if (0 == timer_) {
		/*
		 * Init the timer with 60 mS period and 1 MHz frequency. This should give us
		 * resolution of 1 uS.
		 */
		timer_ = new HwTimer(timer_id, TimeSpan::from_milliseconds(60),
				Frequency::from_megahertz(10));
	}
	return !!timer_ ? timer_->start() : false;
}

void TimeStamp::Cleanup()
{
	if (timer_) {
		delete timer_;
		timer_ = 0;
	}
}

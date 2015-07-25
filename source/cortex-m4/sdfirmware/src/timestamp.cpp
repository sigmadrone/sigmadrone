/*
 * timestamp.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: svetlio
 */

#include "timestamp.h"

HwTimer* TimeStamp::timer_ = 0;

static TimeStamp boot_ts;

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
	Stamp now;
	do {
		now = read_timer();
	} while (!(now.current_elapsed_count_ > stamp_.current_elapsed_count_ ||
		now.current_period_ >= stamp_.current_period_));
	if (now.current_elapsed_count_ == stamp_.current_elapsed_count_) {
		ts_elapsed = now.current_period_ - stamp_.current_period_;
	} else {
		assert(now.current_elapsed_count_ > stamp_.current_elapsed_count_);
		ts_elapsed = timer_->period() - stamp_.current_period_ + now.current_period_;
		ts_elapsed += timer_->period() * (now.current_elapsed_count_ - stamp_.current_elapsed_count_ - 1);
	}
	return ts_elapsed;
}

bool TimeStamp::init(HwTimer::Id timer_id)
{
	assert(timer_id != HwTimer::TIMER_1 &&
			timer_id != HwTimer::TIMER_8);
	if (0 == timer_) {
		/*
		 * Init the timer with 60 mS period and 1 MHz frequency. This should give us
		 * resolution of 1 uS.
		 */
		timer_ = new HwTimer(timer_id, TimeSpan::from_microseconds(1000),
				Frequency::from_megahertz(45));
	}
	return !!timer_ ? timer_->start() : false;
}

void TimeStamp::cleanup()
{
	if (timer_) {
		delete timer_;
		timer_ = 0;
	}
}

TimeSpan TimeStamp::time_lost_per_time_stamp()
{
	static const size_t count = 10000;

	TimeStamp ts;
	TimeSpan base_line;

	ts.time_stamp();
	volatile int cnt = 0;
	for (size_t i = 0; i < count; i++) {
		++cnt;
	}
	base_line = ts.elapsed();

	TimeStamp ts2;
	ts.time_stamp();
	for (size_t i = 0; i < count; i++) {
		++cnt;
		ts2.elapsed();
	}
	return (ts.elapsed() - base_line) / count;
}

TimeSpan TimeStamp::since_boot() {
	return boot_ts.elapsed();
}

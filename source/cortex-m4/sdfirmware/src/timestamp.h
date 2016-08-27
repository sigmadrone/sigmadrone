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

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "hwtimer.h"

class TimeStamp {
public:
	static bool init(HwTimer::Id timer_id = HwTimer::TIMER_7);
	static void cleanup();
	static TimeSpan time_lost_per_time_stamp();
	static TimeSpan since_boot();

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
};

class PerfCounter {
public:
	PerfCounter() : iterations_(0) {}
	void begin_measure()  {  timestamp_.time_stamp(); }
	void end_measure()
	{
		total_elapsed_ += timestamp_.elapsed();
		++iterations_;
		average_ = total_elapsed_ / iterations_;
	}
	TimeSpan total_elapsed()  { return total_elapsed_; }
	TimeSpan average_elapsed() { return average_; }
private:
	TimeSpan total_elapsed_;
	TimeSpan average_;
	TimeStamp timestamp_;
	uint32_t iterations_;
};

#endif /* TIMESTAMP_H_ */

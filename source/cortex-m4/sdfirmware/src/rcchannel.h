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

#ifndef RCCHANNEL_H_
#define RCCHANNEL_H_

#include "pwmdecoder.h"
#include "rcdefs.h"

class RcChannel {
public:
	RcChannel(const PwmRxConsts& pwmRxConsts, FunctionPointer callback);
	PwmDecoder& decoder();
	void start_receive();
	void stop_receive();
	bool is_live() const;

private:
	RcChannel(const RcChannel&);
	const RcChannel& operator=(const RcChannel&);

private:
	PwmDecoder decoder_;
	uint32_t tim_period_channel_;
};

#endif /* RCCHANNEL_H_ */

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

#ifndef RCRECEIVER_H_
#define RCRECEIVER_H_

#include "rcchannel.h"

class RcReceiver {
public:
	RcReceiver(const std::vector<PwmRxConsts> rxConsts, FunctionPointer callback = FunctionPointer());
	RcChannel* channel(uint32_t channelno);
	void start();
	void stop();
	size_t channel_count() { return channels_.size(); }
private:
	std::vector<RcChannel*> channels_;
};

#endif /* RCRECEIVER_H_ */

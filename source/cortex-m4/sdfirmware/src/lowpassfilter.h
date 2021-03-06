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

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

template <typename DataType, typename CoeffType>
class LowPassFilter
{
public:
	LowPassFilter(CoeffType alpha) : alpha_(alpha), beta_(1.0 - alpha) { reset(); }
	LowPassFilter(CoeffType alpha, CoeffType beta) : alpha_(alpha), beta_(beta) { reset(); }
	~LowPassFilter() = default;

	void reset(const DataType& out = {0})
	{
		out_ = out;
	}

	const DataType& do_filter(const DataType& in)
	{
		out_ = out_ * alpha_ + in * beta_;
		return out_;
	}

	const DataType& output() const
	{
		return out_;
	}

	void set_alpha(const CoeffType& alpha)
	{
		alpha_ = alpha;
		beta_ = 1.0 - alpha;
	}

	CoeffType alpha() { return alpha_; }
private:
	DataType out_;
	CoeffType alpha_, beta_;
};

#endif

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

#ifndef INCLUDE_FIXEDPOINT_H_
#define INCLUDE_FIXEDPOINT_H_

namespace mul64 {
inline void umul64wide (uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo);
inline void mul64wide (int64_t a, int64_t b, int64_t *hi, int64_t *lo);
/* compute mul_wide (a, b) >> s, for s in [0,63] */
inline int64_t mul64wideshift (int64_t a, int64_t b, int s);
}

template <typename T> struct QmnAllowedTypes;

template<> struct QmnAllowedTypes <int8_t> {};
template<> struct QmnAllowedTypes <int16_t> {};
template<> struct QmnAllowedTypes <int32_t> {};
template<> struct QmnAllowedTypes <int64_t> {};

template <typename T=int32_t, const size_t N=31>
class QmnNumber : public QmnAllowedTypes<T> {
public:
	// construction
	QmnNumber();
	QmnNumber(int int_part);
	QmnNumber(float f);
	QmnNumber(double d);
	static QmnNumber from_int64(int64_t i);
	~QmnNumber() = default;

	// Copy semantics
	QmnNumber(const QmnNumber&) = default;
	QmnNumber& operator=(const QmnNumber&) = default;

	// operators
	const QmnNumber& operator+=(const QmnNumber& rhs);
	const QmnNumber& operator-=(const QmnNumber& rhs);
	const QmnNumber& operator*=(const QmnNumber& rhs);
	const QmnNumber& operator/=(const QmnNumber& rhs);
	QmnNumber operator+(const QmnNumber& qmn) const {return QmnNumber(*this).operator+=(qmn);}
	QmnNumber operator+(float f) const {return operator+(QmnNumber(f));}
	QmnNumber operator+(double d) const {return operator+(QmnNumber(d));}
	QmnNumber operator+(int i) const {return operator+(QmnNumber(i));}
	QmnNumber operator-(const QmnNumber& qmn) const {return QmnNumber(*this).operator-=(qmn);}
	QmnNumber operator-(float f) const {return operator-(QmnNumber(f));}
	QmnNumber operator-(double d) const {return operator-(QmnNumber(d));}
	QmnNumber operator-(int i) const {return operator-(QmnNumber(i));}
	QmnNumber operator*(const QmnNumber& qmn) const {return QmnNumber(*this).operator*=(qmn);}
	QmnNumber operator*(float f) const {return operator*(QmnNumber(f));}
	QmnNumber operator*(double d) const {return operator*(QmnNumber(d));}
	QmnNumber operator*(int i) const {return operator*(QmnNumber(i));}
	QmnNumber operator/(const QmnNumber& qmn) const {return QmnNumber(*this).operator/=(qmn);}
	QmnNumber operator/(float f) const {return operator/(QmnNumber(f));}
	QmnNumber operator/(double d) const {return operator/(QmnNumber(d));}
	QmnNumber operator/(int i) const {return operator/(QmnNumber(i));}
	const QmnNumber& operator-() { qmn_number_ = -qmn_number_; return *this; }
	operator double() const { return to_float<double>(); }
	operator float() const { return to_float<float>(); }

	static double eps() { return 1.0 / static_cast<double>(mult_factor_); }
	static float eps_float() { return static_cast<float>(eps());}

private:
	template<typename Value> void init(Value v);
	template<typename F> F to_float() const;
	static T saturate_add_sub(T a, T b, T result);
	static T saturate(int64_t integer, T max_int, T min_int);
	static inline int64_t min(int64_t a, int64_t b) { return a < b ? a : b; }
	static inline int64_t max(int64_t a, int64_t b) { return a > b ? a : b; }

private:
	static const size_t bitsize_ = sizeof(T)*8;
	static const T sign_bit_ = static_cast<T>(1) << (bitsize_-1);
	static const uint64_t mult_factor_ = static_cast<uint64_t>(1) << N;
	static const T min_value_ = static_cast<T>(1) << (bitsize_-1);
	static const T max_value_ = ~min_value_;
	static const T max_intpart_ = (static_cast<T>(1) << (bitsize_-N-1))-1;
	static const T min_intpart_ = -max_intpart_-1;

	T qmn_number_;
};

typedef QmnNumber<> Q1dot31;
typedef QmnNumber<int64_t, 63> Q1dot63;
typedef QmnNumber<int64_t, 40> Q24dot40;
typedef QmnNumber<int64_t, 32> Q32dot32;
typedef QmnNumber<int64_t, 48> Q16dot48;

template <typename T, const size_t N>
QmnNumber<T, N>::QmnNumber()
{
	init(0);
}

template <typename T, const size_t N>
QmnNumber<T, N>::QmnNumber(int int_part)
{
	init(int_part);
}

template <typename T, const size_t N>
QmnNumber<T, N>::QmnNumber(float f)
{
	init(f);
}

template <typename T, const size_t N>
QmnNumber<T, N>::QmnNumber(double d)
{
	init(d);
}

template <typename T, const size_t N>
QmnNumber<T,N> QmnNumber<T,N>::from_int64(int64_t i)
{
	QmnNumber q;
	q.init(static_cast<T>(i));
	return q;
}

template <typename T, const size_t N> template<typename Value>
void QmnNumber<T, N>::init(Value v)
{
	static_assert(N < bitsize_, "The integer part must be at least one bit in size");
	if (v >= max_intpart_) {
		qmn_number_ = max_value_;
	} else if (v <= min_intpart_) {
		qmn_number_ = min_value_;
	} else {
		qmn_number_ = static_cast<T>(v * mult_factor_);
	}
}

template <typename T, const size_t N> template<typename F>
F QmnNumber<T, N>::to_float() const
{
	return static_cast<F>(qmn_number_) / static_cast<F>(mult_factor_);
}

template <typename T, const size_t N>
T QmnNumber<T, N>::saturate(int64_t integer, T max_int, T min_int)
{
	return static_cast<T>(max(min_int, min(max_int,integer)));
}

template <typename T, const size_t N>
T QmnNumber<T, N>::saturate_add_sub(T a, T b, T result)
{
	if (!((a & sign_bit_) ^ (b & sign_bit_)) && ((a & sign_bit_) ^ (result & sign_bit_))) {
		return a > 0 ? max_value_ : min_value_;
	}
	return result;
}

template <typename T, const size_t N>
const QmnNumber<T,N>& QmnNumber<T, N>::operator+=(const QmnNumber& rhs)
{
	qmn_number_ = saturate_add_sub(qmn_number_, rhs.qmn_number_, qmn_number_ + rhs.qmn_number_);
	return *this;
}

template <typename T, const size_t N>
const QmnNumber<T,N>& QmnNumber<T, N>::operator-=(const QmnNumber& rhs)
{
	qmn_number_ = saturate_add_sub(qmn_number_, -rhs.qmn_number_, qmn_number_ - rhs.qmn_number_);
	return *this;
}

template <typename T, const size_t N>
const QmnNumber<T,N>& QmnNumber<T, N>::operator*=(const QmnNumber& rhs)
{
	T prod;
	if (bitsize_ < 64) {
		prod = static_cast<T>((static_cast<int64_t>(qmn_number_) * rhs.qmn_number_) >> N);
	} else {
		//prod = static_cast<T>((static_cast<__int128_t>(qmn_number_) * rhs.qmn_number_) >> N);
		prod = mul64::mul64wideshift(qmn_number_, rhs.qmn_number_, N);
	}
	if (!((qmn_number_ & sign_bit_) ^ (rhs.qmn_number_ & sign_bit_))) {
		if (prod < 0) {
			prod = max_value_;
		}
	} else if (prod > 0) {
		prod = min_value_;
	}
	qmn_number_ = prod;
	return *this;
}

template <typename T, const size_t N>
const QmnNumber<T,N>& QmnNumber<T, N>::operator/=(const QmnNumber& rhs)
{
	if (bitsize_ < 64) {
		qmn_number_ = static_cast<T>((static_cast<int64_t>(qmn_number_) << N) / rhs.qmn_number_);
	} else {
		//qmn_number_ = static_cast<T>((static_cast<__int128_t>(qmn_number_) << N) / rhs.qmn_number_);
		assert(false);
	}
	return *this;
}

namespace mul64 {
inline void umul64wide (uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo)
{
    uint64_t a_lo = (uint64_t)(uint32_t)a;
    uint64_t a_hi = a >> 32;
    uint64_t b_lo = (uint64_t)(uint32_t)b;
    uint64_t b_hi = b >> 32;

    uint64_t p0 = a_lo * b_lo;
    uint64_t p1 = a_lo * b_hi;
    uint64_t p2 = a_hi * b_lo;
    uint64_t p3 = a_hi * b_hi;

    uint32_t cy = (uint32_t)(((p0 >> 32) + (uint32_t)p1 + (uint32_t)p2) >> 32);

    *lo = p0 + (p1 << 32) + (p2 << 32);
    *hi = p3 + (p1 >> 32) + (p2 >> 32) + cy;
}

inline void mul64wide (int64_t a, int64_t b, int64_t *hi, int64_t *lo)
{
    umul64wide ((uint64_t)a, (uint64_t)b, (uint64_t *)hi, (uint64_t *)lo);
    if (a < 0LL) *hi -= b;
    if (b < 0LL) *hi -= a;
}

/* compute mul_wide (a, b) >> s, for s in [0,63] */
inline int64_t mul64wideshift (int64_t a, int64_t b, int s)
{
    int64_t res;
    int64_t hi, lo;
    mul64wide (a, b, &hi, &lo);
    if (s) {
        res = ((uint64_t)hi << (64 - s)) | ((uint64_t)lo >> s);
    } else {
        res = lo;
    }
    return res;
}
}


#endif /* INCLUDE_FIXEDPOINT_H_ */

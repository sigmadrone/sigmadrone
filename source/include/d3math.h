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

#ifndef __D3MATH_H__
#define __D3MATH_H__

#include <sstream>
#include <cassert>
#include <cmath>
#include <array>
#include <type_traits>
#include <numeric>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef DEG2RAD
#define DEG2RAD(__d__) (((__d__) * M_PI) / 180.0)
#define RAD2DEG(__r__) (((__r__) / M_PI) * 180.0)
#endif

#ifdef D3MATH_NAMESPACE
namespace D3MATH_NAMESPACE
{
#endif

const double EPSILON = 4.37114e-05;

template<typename T>
class Quaternion;

template <typename T = float, size_t ROWS = 1, size_t COLS = 1>
class MatrixBase {
public:
	using array_type 		= std::array<T, ROWS * COLS>;
	using value_type 		= typename array_type::value_type;
    using iterator			= typename array_type::iterator;
    using const_iterator	= typename array_type::const_iterator;

public:
	array_type data;
	static constexpr size_t rows = ROWS;
	static constexpr size_t cols = COLS;

	MatrixBase();
	MatrixBase(const std::initializer_list<std::initializer_list<T>>& list);
	MatrixBase(const std::initializer_list<T>& list);

	template<typename... ARGS>
	explicit MatrixBase(ARGS... args);

	template<typename... ARGS>
	void init(iterator& it, T x, ARGS... args);

	void init(iterator& it, T x);

    // Move semantics
	MatrixBase(MatrixBase&&) = default;
	MatrixBase& operator=(MatrixBase&&) = default;

    // Copy semantics
	MatrixBase(const MatrixBase&) = default;
	MatrixBase& operator=(const MatrixBase&) = default;

    // Destruction
    ~MatrixBase() = default;

	T& at(size_t row, size_t col);
	const T& at(size_t row, size_t col) const;
	std::string to_string() const;
	void clear();
	void swap_rows(size_t m, size_t n);
	void swap_columns(size_t m, size_t n);
	T sum() const;
	T min() const;
	T max() const;

	void set_row(size_t m, const MatrixBase<T, 1, COLS>& r);
	void set_column(size_t n, const MatrixBase<T, ROWS, 1>& c);
	MatrixBase<T, 1, COLS> row(size_t m) const;
	MatrixBase<T, ROWS, 1> column(size_t n) const;
	MatrixBase<T, COLS, ROWS> transpose() const;

	T* operator[](size_t row)								{ return &data[row * COLS]; }
	const T* operator[](size_t row)	const					{ return &data[row * COLS]; }
	operator const T*()	const								{ return &data[0];			}
	operator T*()											{ return &data[0];			}

    // Scalar arithmetic
	MatrixBase& operator=(const T& x);
	MatrixBase& operator+=(const T& x);
	MatrixBase& operator-=(const T& x);
	MatrixBase& operator*=(const T& x);
	MatrixBase& operator/=(const T& x);
	MatrixBase& operator%=(const T& x);
	MatrixBase operator+(const T& x) const					{ MatrixBase ret(*this); ret += x; return ret; }
	MatrixBase operator-(const T& x) const					{ MatrixBase ret(*this); ret -= x; return ret; }
	MatrixBase operator*(const T& x) const					{ MatrixBase ret(*this); ret *= x; return ret; }
	MatrixBase operator/(const T& x) const					{ MatrixBase ret(*this); ret /= x; return ret; }
	MatrixBase operator%(const T& x) const					{ MatrixBase ret(*this); ret %= x; return ret; }

	// Matrix arithmetic
	MatrixBase& operator-=(const MatrixBase& m);
	MatrixBase& operator+=(const MatrixBase& m);
	MatrixBase& operator*=(const MatrixBase& m)				{ return (*this = operator*(m)); }
	MatrixBase operator-(const MatrixBase& m) const			{ MatrixBase ret(*this); ret -= m; return ret; }
	MatrixBase operator+(const MatrixBase& m) const			{ MatrixBase ret(*this); ret += m; return ret; }

	iterator begin() 										{ return data.begin(); }
	iterator end()   										{ return data.end(); }
	const_iterator begin() const 							{ return data.begin(); }
	const_iterator end() const   							{ return data.end(); }

	template<typename rhsT, size_t rhsCOLS>
	MatrixBase<T, ROWS, rhsCOLS> operator*(const MatrixBase<rhsT, COLS, rhsCOLS>& rhs) const;

	bool lup(MatrixBase<T, ROWS, COLS> &L, MatrixBase<T, ROWS, COLS> &U, MatrixBase<T, ROWS, COLS> &P) const;
	MatrixBase<T, ROWS, COLS> inverse() const;

	template <typename TT, size_t RR, size_t CC>
	friend std::ostream& operator<<(std::ostream& os, const MatrixBase<TT, RR, CC>& m);

	template<size_t RR = ROWS, size_t CC = COLS>
	static typename std::enable_if<RR == CC, MatrixBase<T, ROWS, COLS>>::type identity();

protected:
	template <typename Functor>
	MatrixBase& apply(Functor f);

	template <typename M, typename Functor>
	MatrixBase& apply(const M& m, Functor f);

	static T abs(T a) { return (a < 0) ? -a : a; }
};

template <typename T, size_t ROWS, size_t COLS>
class MatrixMN : public MatrixBase<T, ROWS, COLS>
{
public:
	using base = MatrixBase<T, ROWS, COLS>;

public:
	MatrixMN() : MatrixBase<T, ROWS, COLS>() {} ;
	MatrixMN(const std::initializer_list<std::initializer_list<T>>& list) : MatrixBase<T, ROWS, COLS>(list) {}
	MatrixMN(const std::initializer_list<T>& list) : MatrixBase<T, ROWS, COLS>(list) {}
	MatrixMN(const MatrixBase<T, ROWS, COLS>& m) : MatrixBase<T, ROWS, COLS>(m) {} ;

	template<typename... ARGS>
	explicit MatrixMN(ARGS... args) : MatrixBase<T, ROWS, COLS>(args...) {};

	MatrixMN& operator=(const T& x);
	MatrixMN operator-(const MatrixMN& m) const;
	MatrixMN operator+(const MatrixMN& m) const;
};


template <typename T, size_t ROWS>
class MatrixMN<T, ROWS, 1> : public MatrixBase<T, ROWS, 1>
{
public:
	using base = MatrixBase<T, ROWS, 1>;

public:
	MatrixMN() : MatrixBase<T, ROWS, 1>() {}
	MatrixMN(const std::initializer_list<std::initializer_list<T>>& list) : MatrixBase<T, ROWS, 1>(list) {}
	MatrixMN(const std::initializer_list<T>& list) : MatrixBase<T, ROWS, 1>(list) {}
	MatrixMN(const MatrixBase<T, ROWS, 1>& m) : MatrixBase<T, ROWS, 1>(m) {}

	template<typename... ARGS>
	explicit MatrixMN(ARGS... args) : MatrixBase<T, ROWS, 1>(args...) {}

	T dot(const MatrixMN& v);
	MatrixMN<T, 3, 1> cross(const MatrixMN<T, 3, 1>& v) const;
	MatrixMN<T, 3, 1> projection(const MatrixMN<T, 3, 1>& u) const;
	MatrixMN<T, 3, 1> perpendicular(const MatrixMN<T, 3, 1>& u) const;
	MatrixMN<T, 3, 1> parallel(const MatrixMN<T, 3, 1>& v) const;
	MatrixMN<T, 3, 1> perpendicular2(const MatrixMN<T, 3, 1>& v) const;
	MatrixMN<T, 3, 1> reflection(const MatrixMN<T, 3, 1>& v) const;

	MatrixMN operator-(const MatrixMN& m) const;
	MatrixMN operator+(const MatrixMN& m) const;
	MatrixMN& operator=(const MatrixMN& m);
	MatrixMN& operator=(const T& x);
	T length_squared() const;
	T length() const;
	MatrixMN normalize() const;
	T& at(size_t row);
	const T& at(size_t row) const;
	T& operator[](size_t row);
	const T& operator[](size_t row)	const;
	T& x();
	const T& x() const;
	T& y();
	const T& y() const;
	T& z();
	const T& z() const;
	T& w();
	const T& w() const;

	static T dot(const MatrixMN& u, const MatrixMN& v) { return MatrixMN(u).dot(v); }
	static MatrixMN<T, 3, 1> cross(const MatrixMN<T, 3, 1>& u, const MatrixMN<T, 3, 1>& v) { return MatrixMN(u).cross(v); }
};


template<typename T>
class Quaternion
{
public:
	T w, x, y, z;

public:
	static Quaternion<T> identity;

	Quaternion(const Quaternion<T>& q);
	Quaternion(const MatrixMN<T, 3, 1>& v, T _w = 0);
	Quaternion(T _w = 0, T _x = 0, T _y = 0, T _z = 0);

	template<class FromT>
	Quaternion(const Quaternion<FromT>& q);

	template<typename FromT>
	Quaternion<T>& operator=(const Quaternion<FromT>& rhs);

	Quaternion<T> operator+(const Quaternion<T>& rhs) const;
	Quaternion<T> operator*(T rhs) const;
	Quaternion<T> operator/(T rhs) const;
	Quaternion<T> operator-(const Quaternion<T>& rhs) const;
	Quaternion<T> operator*(const Quaternion<T>& rhs) const;
	Quaternion<T>& operator+=(const Quaternion<T>& rhs);
	Quaternion<T>& operator*=(T rhs);
	Quaternion<T>& operator-=(const Quaternion<T>& rhs);
	Quaternion<T>& operator*=(const Quaternion<T>& rhs);
	bool operator==(const Quaternion<T>& rhs) const;
	bool operator!=(const Quaternion<T>& rhs) const;
	Quaternion<T> operator-() const;
	Quaternion<T> operator~() const;
	Quaternion<T> conjugate() const;
	T length() const;
	T lengthSq() const;
	Quaternion<T> normalize() const;
	MatrixMN<T, 3, 1> rotate(const MatrixMN<T, 3, 1>& v) const;
	Quaternion<T> ln() const;
	Quaternion<T> exp() const;
	double angle() const;
	MatrixMN<T,3,1> axis() const;
	MatrixMN<T, 4, 4> rotMatrix4() const;
	MatrixMN<T, 3, 3> rotMatrix3() const;
	Quaternion<T> reciprocal();
	std::string toString() const;

	static T dot(const Quaternion<T>& u, const Quaternion<T>& v);
	static T theta(const Quaternion<T>& u, const Quaternion<T>& v);
	static Quaternion<T> fromAngularVelocity(const MatrixMN<T,3,1>& omega /* Rad/Sec */, float deltaT);
	static Quaternion<T> fromAxisRot(MatrixMN<T,3,1> axis, float rad);
	static Quaternion<T> fromEulerAngles(T x, T y, T z);
	static Quaternion<T> fromVectors(const MatrixMN<T, 3, 1>& u, const MatrixMN<T, 3, 1>& v);
	static Quaternion<T> fromVectorsPartial(const MatrixMN<T,3,1>& u, const MatrixMN<T,3,1>& v, float part);
	static Quaternion<T> nlerp(const Quaternion<T>& i, const Quaternion<T>& f, float alpha);
	static Quaternion<T> slerp(const Quaternion<T>& v0, const Quaternion<T>& v1, float alpha);
	static MatrixMN<T,3,1> angularVelocity(Quaternion<T> i, Quaternion<T> f, float dT);
	static void decomposeTwistSwing(
			const Quaternion<T>& rotation,
			const MatrixMN<T, 3, 1>& direction,
			Quaternion<T>& swing,
			Quaternion<T>& twist);
	static void decomposeSwingTwist(
			const Quaternion<T>& rotation,
			const MatrixMN<T, 3, 1>& direction,
			Quaternion<T>& swing,
			Quaternion<T>& twist);

	template <typename TT>
	friend std::ostream& operator<<(std::ostream& os, const Quaternion<TT>& q);
};

template<class T>
Quaternion<T> Quaternion<T>::identity(1, 0, 0, 0);

template <typename T>
Quaternion<T>::Quaternion(const Quaternion<T>& q)
		: w(q.w), x(q.x), y(q.y), z(q.z)
{
}

template <typename T>
template<class FromT>
Quaternion<T>::Quaternion(const Quaternion<FromT>& q)
		: w(static_cast<T>(q.w)), x(static_cast<T>(q.x)), y(static_cast<T>(q.y)), z(static_cast<T>(q.z))
{
}

template <typename T>
Quaternion<T>::Quaternion(const MatrixMN<T, 3, 1>& v, T _w)
		: w(_w), x(v.at(0)), y(v.at(1)), z(v.at(2))
{
}

template <typename T>
Quaternion<T>::Quaternion(T _w, T _x, T _y, T _z)
		: w(_w), x(_x), y(_y), z(_z)
{
}

template <typename T>
template<typename FromT>
Quaternion<T>& Quaternion<T>::operator=(const Quaternion<FromT>& rhs)
{
	w = static_cast<T>(rhs.w);
	x = static_cast<T>(rhs.x);
	y = static_cast<T>(rhs.y);
	z = static_cast<T>(rhs.z);
	return *this;
}


template <typename T>
Quaternion<T> Quaternion<T>::operator+(const Quaternion<T>& rhs) const
{
	Quaternion<T>ret(*this);
	ret += rhs;
	return ret;
}

template<typename T>
Quaternion<T> Quaternion<T>::operator*(T rhs) const
{
	Quaternion<T> ret(*this);
	ret *= rhs;
	return ret;
}

template<typename T>
Quaternion<T> Quaternion<T>::operator/(T rhs) const
{
	Quaternion<T> ret(*this);
	ret /= rhs;
	return ret;
}

template<typename T>
Quaternion<T> Quaternion<T>::operator-(const Quaternion<T>& rhs) const
{
	Quaternion<T> ret(*this);
	ret -= rhs;
	return ret;
}

template <typename T>
Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& rhs) const
{
	return Quaternion<T>(w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
			w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w);
}

template <typename T>
Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& rhs)
{
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

template <typename T>
Quaternion<T>& Quaternion<T>::operator*=(T rhs)
{
	w *= rhs;
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

template <typename T>
Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& rhs)
{
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

template <typename T>
Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& rhs)
{
	Quaternion q = (*this) * rhs;
	*this = q;
	return *this;
}

template <typename T>
bool Quaternion<T>::operator==(const Quaternion<T>& rhs) const
{
	const Quaternion<T>& lhs = *this;
	return (std::fabs(w - rhs.w) < EPSILON) &&
			(std::fabs(x - rhs.x) < EPSILON) &&
			(std::fabs(y - rhs.y) < EPSILON) &&
			(std::fabs(z - rhs.z) < EPSILON);
}

template <typename T>
bool Quaternion<T>::operator!=(const Quaternion<T>& rhs) const
{
	return !(*this == rhs);
}

template <typename T>
Quaternion<T> Quaternion<T>::operator-() const
{
	return Quaternion<T>(-w, -x, -y, -z);
}

template <typename T>
Quaternion<T> Quaternion<T>::operator~() const
{
	return conjugate();
}


template <typename T>
Quaternion<T> Quaternion<T>::conjugate() const
{
	return Quaternion<T>(w, -x, -y, -z);
}

template <typename T>
T Quaternion<T>::length() const
{
	return static_cast<T>(std::sqrt(w * w + x * x + y * y + z * z));
}

template <typename T>
T Quaternion<T>::lengthSq() const
{
	return static_cast<T>(w * w + x * x + y * y + z * z);
}

template <typename T>
Quaternion<T> Quaternion<T>::normalize() const
{
	Quaternion<T> q = *this;
	T len = length();
	if (len > EPSILON) {
		q.w /= len;
		q.x /= len;
		q.y /= len;
		q.z /= len;
	}
	return q;
}

template <typename T>
MatrixMN<T, 3, 1> Quaternion<T>::rotate(const MatrixMN<T, 3, 1>& v) const
{
	Quaternion<T> r = (*this)*Quaternion<T>(0, v.at(0), v.at(1), v.at(2))*~(*this);
	return MatrixMN<T, 3, 1>(r.x, r.y, r.z);
}

template <typename T>
Quaternion<T> Quaternion<T>::ln() const
{
	Quaternion<T> Q(*this);
	MatrixMN<T,3,1> v = MatrixMN<T,3,1>(Q.x, Q.y, Q.z);
	T lV = v.length();
	double ac = std::acos(Q.w/Q.length());
	return Quaternion<T>(std::log(Q.length()), v.at(0)/lV*ac, v.at(1)/lV*ac, v.at(2)/lV*ac);
}

template <typename T>
Quaternion<T> Quaternion<T>::exp() const
{
	MatrixMN<T,3,1> v(x, y, z);
	T lV = v.length();
	double c = std::cos(lV);
	double s = std::sin(lV);
	double e = std::exp(w);
	return Quaternion<T>(e*c, e*v.at(0)/lV*s, e*v.at(1)/lV*s, e*v.at(2)/lV*s);
}

template <typename T>
double Quaternion<T>::angle() const
{
    Quaternion<T> q = this->normalize();
    return static_cast<double>(2*std::acos(q.w));
}

template <typename T>
MatrixMN<T,3,1> Quaternion<T>::axis() const
{
	Quaternion<T> q = this->normalize();

	double ca2 = q.w;
	double sa2  = std::sqrt( 1.0 - ca2 * ca2 );
	if (fabs( sa2 ) < 0.0005)
	  sa2 = 1;
	return MatrixMN<T,3,1>(q.x/sa2, q.y/sa2, q.z/sa2);
}

template<typename T>
MatrixMN<T, 3, 3> Quaternion<T>::rotMatrix3() const
{
	MatrixMN<T,3,3> ret;
	Quaternion<T> q = normalize();

	T xx = q.x * q.x;
	T xy = q.x * q.y;
	T xz = q.x * q.z;
	T xw = q.x * q.w;
	T yy = q.y * q.y;
	T yz = q.y * q.z;
	T yw = q.y * q.w;
	T zz = q.z * q.z;
	T zw = q.z * q.w;

	ret.at(0, 0) = 1 - 2 * (yy + zz);
	ret.at(0, 1) = 2 * (xy - zw);
	ret.at(0, 2) = 2 * (xz + yw);

	ret.at(1, 0) = 2 * (xy + zw);
	ret.at(1, 1) = 1 - 2 * (xx + zz);
	ret.at(1, 2) = 2 * (yz - xw);

	ret.at(2, 0) = 2 * (xz - yw);
	ret.at(2, 1) = 2 * (yz + xw);
	ret.at(2, 2) = 1 - 2 * (xx + yy);
	return ret;
}

template<typename T>
MatrixMN<T, 4, 4> Quaternion<T>::rotMatrix4() const
{
	MatrixMN<T,4,4> ret4;
	MatrixMN<T,3,3> ret3 = rotMatrix3();

	ret4[0][0] = ret3[0][0];
	ret4[0][1] = ret3[0][1];
	ret4[0][2] = ret3[0][2];

	ret4[1][0] = ret3[1][0];
	ret4[1][1] = ret3[1][1];
	ret4[1][2] = ret3[1][2];

	ret4[2][0] = ret3[2][0];
	ret4[2][1] = ret3[2][1];
	ret4[2][2] = ret3[2][2];
	ret4[3][3] = 1;
	return ret4;
}

template <typename TT>
std::ostream& operator<<(std::ostream& os, const Quaternion<TT>& q)
{
	os.setf(std::ios::fixed, std::ios::floatfield);
	os.precision(2);
	os << std::showpos << q.w << std::showpos << q.x << "i" << std::showpos << q.y << "j" << std::showpos << q.z << "k";
	return os;
}

template <typename T>
std::string Quaternion<T>::toString() const
{
	std::stringstream oss;
	oss << *this;
	return oss.str();
}

template <typename T>
MatrixMN<T,3,1> Quaternion<T>::angularVelocity(Quaternion<T> i, Quaternion<T> f, float dT)
{
	double dotprod = dot(i, f);
	if (dotprod < 0.0)
		f = -f;
	if (dotprod < 1.0f && dotprod > -1.0f) {
		Quaternion<T> logQ = (f * ~i).ln();
		MatrixMN<T,3,1> V = MatrixMN<T,3,1>(logQ.x, logQ.y, logQ.z) * 2.0f / dT;
		return V;
	}
	return MatrixMN<T,3,1>(0, 0, 0);
}


template <typename T>
Quaternion<T> Quaternion<T>::reciprocal()
{
	Quaternion<T> rec;

	T norm = length();
	if (0 != norm) {
		rec = conjugate() / (norm * norm);
	} else {
		rec = *this;
	}
	return rec;
}

template <typename T>
Quaternion<T> Quaternion<T>::slerp(const Quaternion<T>& v0, const Quaternion<T>& v1, float alpha)
{
    float dot = Quaternion<T>::dot(v0, v1);
    const float DOT_THRESHOLD = 0.9995f;

    if (dot > DOT_THRESHOLD)
        return nlerp(v0, v1, alpha);

    /* clamp dot between -1.0 and 1.0 */
    dot = std::min(std::max(dot, -1.0f), 1.0f);
    float theta_0 = acosf(dot);
    float theta = theta_0 * alpha;
    Quaternion<T> v2 = (v1 - v0 * dot).normalize();
    return v0*cos(theta) + v2*sin(theta);
}

template <typename T>
Quaternion<T> Quaternion<T>::fromAngularVelocity(const MatrixMN<T,3,1>& omega /* Rad/Sec */, float deltaT)
{
	Quaternion<T> deltaQ;
	MatrixMN<T,3,1> theta = omega * (0.5f * deltaT);
	float thetaMagSq = theta.length_squared();
	float s;

	if(thetaMagSq * thetaMagSq / 24.0f < EPSILON) {
		deltaQ.w = 1.0f - thetaMagSq / 2.0f;
		s = 1.0f - thetaMagSq / 6.0f;
	} else {
		float thetaMag = std::sqrt(thetaMagSq);
		deltaQ.w = std::cos(thetaMag);
		s = sin(thetaMag) / thetaMag;
	}

	deltaQ.x = theta.x() * s;
	deltaQ.y = theta.y() * s;
	deltaQ.z = theta.z() * s;

	return deltaQ;
}

template <typename T>
Quaternion<T> Quaternion<T>::fromAxisRot(MatrixMN<T,3,1> axis, float rad)
{
	double sa2 = std::sin(rad / 2);
	double ca2 = std::cos(rad / 2);

	/*
	 * Constructing quaternion from vector and angle requires the vector portion to be the unit vector.
	 * q = cos(a/2) + i ( x * sin(a/2)) + j (y * sin(a/2)) + k ( z * sin(a/2))
	 */
	axis = axis.normalize() * sa2;
	Quaternion<T> temp(ca2, axis.at(0), axis.at(1), axis.at(2));
	return temp;
}

template <typename T>
Quaternion<T> Quaternion<T>::fromEulerAngles(T x, T y, T z)
{
	Quaternion<T> ret = fromAxisRot(MatrixMN<T,3,1>(1, 0, 0), x) * fromAxisRot(MatrixMN<T,3,1>(0, 1, 0), y)
			* fromAxisRot(MatrixMN<T,3,1>(0, 0, 1), z);
	return ret;
}

template <typename T>
Quaternion<T> Quaternion<T>::fromVectors(const MatrixMN<T, 3, 1>& u, const MatrixMN<T, 3, 1>& v)
{
	// Based on Stan Melax's article in Game Programming Gems
	Quaternion<T> q;
	// Copy, since cannot modify local
	MatrixMN<T, 3, 1> v0 = u.normalize();
	MatrixMN<T, 3, 1> v1 = v.normalize();
	double d = MatrixMN<T, 3, 1>::dot(v0, v1);

	// If dot == 1, vectors are the same
	if (d > 1.0f - EPSILON) {
		return Quaternion<T>(1, 0, 0, 0);
	}
	if (d < (EPSILON - 1.0f)) {
		MatrixMN<T, 3, 1> axis = MatrixMN<T, 3, 1>::cross(MatrixMN<T, 3, 1>(1, 0, 0), v0);
		if (axis.length_squared() < EPSILON * EPSILON) // pick another if colinear
			axis = MatrixMN<T, 3, 1>::cross(MatrixMN<T, 3, 1>(0, 1, 0), v0);
		q = fromAxisRot(axis, M_PI);
	} else {
		double s = std::sqrt((1 + d) * 2);
		double invs = 1 / s;
		MatrixMN<T, 3, 1> c = MatrixMN<T, 3, 1>::cross(v0, v1);
		q.x = c.at(0) * invs;
		q.y = c.at(1) * invs;
		q.z = c.at(2) * invs;
		q.w = s * 0.5f;
		q = q.normalize();
	}
	return q;
}


template <typename T>
Quaternion<T> Quaternion<T>::fromVectorsPartial(const MatrixMN<T,3,1>& u, const MatrixMN<T,3,1>& v, float part)
{
	double d = MatrixMN<T, 3, 1>::dot(u, v);

	// If dot == 1, vectors are the same
	if (d >= (1.0f - EPSILON)) {
		return Quaternion<T>(1, 0, 0, 0);
	}
	Quaternion<T> Q = fromVectors(u,v);
	return fromAxisRot(Q.axis(), part * Q.angle());
}

template <typename T>
T Quaternion<T>::dot(const Quaternion<T>& u, const Quaternion<T>& v)
{
	return u.w*v.w + u.x*v.x + u.y*v.y + u.z*v.z;
}

/**
 * Every rotation can be decomposed to twist and swing
 * That is, first we do the twist around the direction axis
 * then we do the swing around an axis perpendicular to the
 * direction axis.
 * composite_rotation = swing * twist (this has singularity in case of
 * swing rotation close to 180 degrees).
 */
template <typename T>
void Quaternion<T>::decomposeTwistSwing(
		const Quaternion<T>& rotation,
		const MatrixMN<T, 3, 1>& direction,
		Quaternion<T>& swing,
		Quaternion<T>& twist)
{
	MatrixMN<T, 3, 1> rotation_axis(rotation.x, rotation.y, rotation.z);
	// return projection v1 on to v2 (parallel component)
	// here can be optimized if direction is unit
	MatrixMN<T, 3, 1> proj = direction.projection(rotation_axis);
	twist = Quaternion<T>(rotation.w, proj.at(0), proj.at(1), proj.at(2)).normalize();
	swing = rotation * twist.conjugate();
}

template <typename T>
void Quaternion<T>::decomposeSwingTwist(
		const Quaternion<T>& rotation,
		const MatrixMN<T, 3, 1>& direction,
		Quaternion<T>& swing,
		Quaternion<T>& twist)
{
	MatrixMN<T, 3, 1> rotation_axis(rotation.x, rotation.y, rotation.z);
	MatrixMN<T, 3, 1> perp = direction.perpendicular(rotation_axis);
	swing = Quaternion<T>(rotation.w, perp.at(0), perp.at(1), perp.at(2)).normalize();
	twist = rotation * swing.conjugate();
}

/*
 * Angle between two quaternions
 */
template <typename T>
T Quaternion<T>::theta(const Quaternion<T>& u, const Quaternion<T>& v)
{
	return std::acos(std::pow(dot(u, v),2.0)*2.0 - 1.0);
}

template <typename T>
Quaternion<T> Quaternion<T>::nlerp(const Quaternion<T>& i, const Quaternion<T>& f, float alpha)
{
	Quaternion<T> result;

	if(dot(i, f) < 0.0f) {
		result = i * (1.0f - alpha) - f * alpha;
	} else {
		result = i * (1.0f - alpha) + f * alpha;
	}
	return result.normalize();
}



typedef Quaternion<double> QuaternionD;
typedef Quaternion<float> QuaternionF;

typedef MatrixMN<float, 3, 3> Matrix3f;
typedef MatrixMN<double, 3, 3> Matrix3d;
typedef MatrixMN<int, 3, 3> Matrix3i;

typedef MatrixMN<float, 4, 4> Matrix4f;
typedef MatrixMN<double, 4, 4> Matrix4d;
typedef MatrixMN<int, 4, 4> Matrix4i;

typedef MatrixMN<float, 1, 1> Vector1f;
typedef MatrixMN<double, 1, 1> Vector1d;
typedef MatrixMN<int, 1, 1> Vector1i;

typedef MatrixMN<float, 2, 1> Vector2f;
typedef MatrixMN<double, 2, 1> Vector2d;
typedef MatrixMN<int, 2, 1> Vector2i;

typedef MatrixMN<float, 3, 1> Vector3f;
typedef MatrixMN<double, 3, 1> Vector3d;
typedef MatrixMN<int, 3, 1> Vector3i;

typedef MatrixMN<float, 4, 1> Vector4f;
typedef MatrixMN<double, 4, 1> Vector4d;
typedef MatrixMN<int, 4, 1> Vector4i;

template <typename T, size_t ROWS, size_t COLS>
static MatrixBase<T, ROWS, 1> lup_solve(
		const MatrixBase<T, ROWS, COLS> &L,
		const MatrixBase<T, ROWS, COLS> &U,
		const MatrixBase<T, ROWS, COLS> &P,
		const MatrixBase<T, ROWS, 1> &B)
{
	MatrixBase<T, ROWS, 1> X;
	MatrixBase<T, ROWS, 1> Y = P * B;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < i; j++)
			Y.at(i, 0) -= Y.at(j, 0) * L.at(i, j);
	}
	X = Y;
	for (int i = ROWS - 1; i >= 0; i--) {
		for (int j = i + 1; j < ROWS; j++)
			X.at(i, 0) -= X.at(j, 0) * U.at(i, j);
		X.at(i, 0) /= U.at(i,i);
	}
	return X;
}


template <typename T, size_t ROWS, size_t COLS>
template <size_t RR, size_t CC>
typename std::enable_if<RR == CC, MatrixBase<T, ROWS, COLS>>::type MatrixBase<T, ROWS, COLS>::identity()
{
	MatrixBase<T, ROWS, COLS> ret;
	for (size_t i = 0; i < ROWS; i++)
		for (size_t j = 0; j < COLS; j++)
			ret.at(i, j) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
	return ret;
}

template <typename TT, size_t RR, size_t CC>
std::ostream& operator<<(std::ostream& os, const MatrixBase<TT, RR, CC>& m)
{
	os.setf(std::ios::fixed, std::ios::floatfield);
	os.precision(2);
	for (size_t i = 0; i < m.rows; i++) {
		for (size_t j = 0; j < m.cols; j++) {
			os.width(7);
			os << m.at(i, j) << " ";
		}
		os << std::endl;
	}
	return os;
}

template <typename T, size_t ROWS, size_t COLS>
inline std::string MatrixBase<T, ROWS, COLS>::to_string() const
{
	std::stringstream oss;
	oss << *this;
	return oss.str();
}

template <typename T, size_t ROWS, size_t COLS>
MatrixBase<T, COLS, ROWS> MatrixBase<T, ROWS, COLS>::transpose() const
{
	MatrixMN<T, COLS, ROWS> ret;
	for (int i = 0; i < COLS; i++)
		for (int j = 0; j < ROWS; j++)
			ret.at(i, j) = at(j, i);
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
template<typename... ARGS>
void MatrixBase<T, ROWS, COLS>::init(iterator& it, T x, ARGS... args)
{
	assert(it != end());
	*it++ = x;
	init(it, args...);
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixBase<T, ROWS, COLS>::init(iterator& it, T x)
{
	assert(it != end());
	*it++ = x;
}

template <typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>::MatrixBase() {
	for (auto& i : data)
		i = static_cast<T>(0);
}

template <typename T, size_t ROWS, size_t COLS>
template<typename... ARGS>
inline MatrixBase<T, ROWS, COLS>::MatrixBase(ARGS... args) : MatrixBase() {
	auto it = begin();
	init(it, args...);
}

template <typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>::MatrixBase(const std::initializer_list<std::initializer_list<T>>& list) {
	assert(list.size() == ROWS);
	iterator it = data.begin();
	for (auto row : list) {
		assert(row.size() == COLS);
		for (auto elem : row) {
			*it++ = elem;
		}
	}
}

template <typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>::MatrixBase(const std::initializer_list<T>& list) {
	assert(list.size() == ROWS * COLS);
	iterator it = data.begin();
	for (auto elem : list) {
		*it++ = elem;
	}
}

template <typename T, size_t ROWS, size_t COLS>
inline T& MatrixBase<T, ROWS, COLS>::at(size_t row, size_t col)
{
	return data[row * COLS + col];
}

template <typename T, size_t ROWS, size_t COLS>
inline const T& MatrixBase<T, ROWS, COLS>::at(size_t row, size_t col) const
{
	return data[row * COLS + col];
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixBase<T, ROWS, COLS>::clear()
{
	for (size_t i = 0; i < ROWS; i++)
		for (size_t j = 0; j < COLS; j++)
			at(i, j) = static_cast<T>(0);
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixBase<T, ROWS, COLS>::set_row(size_t m, const MatrixBase<T, 1, COLS>& r)
{
	for (int i = 0; i < COLS; i++)
		at(m, i) = r.at(0, i);
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixBase<T, ROWS, COLS>::set_column(size_t n, const MatrixBase<T, ROWS, 1>& c)
{
	for (int i = 0; i < ROWS; i++)
		at(i, n) = c.at(i, 0);
}

template <typename T, size_t ROWS, size_t COLS>
MatrixBase<T, 1, COLS> MatrixBase<T, ROWS, COLS>::row(size_t m) const
{
	MatrixBase<T, 1, COLS> ret;
	for (int i = 0; i < COLS; i++)
		ret.at(0, i) = at(m, i);
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
MatrixBase<T, ROWS, 1> MatrixBase<T, ROWS, COLS>::column(size_t n) const
{
	MatrixBase<T, ROWS, 1> ret;
	for (int i = 0; i < ROWS; i++)
		ret.at(i, 0) = at(i, n);
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixBase<T, ROWS, COLS>::swap_rows(size_t m, size_t n)
{
	T temp;

	assert(m < ROWS && n < ROWS);
	if (m == n)
		return;
	for (int i = 0; i < COLS; i++) {
		temp = at(m, i);
		at(m, i) = at(n, i);
		at(n, i) = temp;
	}
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixBase<T, ROWS, COLS>::swap_columns(size_t m, size_t n)
{
	T temp;

	assert(m < COLS && n < COLS);
	if (m == n)
		return;
	for (int i = 0; i < ROWS; i++) {
		temp = at(i, m);
		at(i, m) = at(i, n);
		at(i, n) = temp;
	}
}

template <typename T, size_t ROWS, size_t COLS>
T MatrixBase<T, ROWS, COLS>::sum() const
{
	return std::accumulate(begin(), end(), static_cast<T>(0));
}

template <typename T, size_t ROWS, size_t COLS>
T MatrixBase<T, ROWS, COLS>::min() const
{
	T ret = *begin();
	for (auto x : data)
		if (x < ret)
			ret = x;
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
T MatrixBase<T, ROWS, COLS>::max() const
{
	T ret = *begin();
	for (auto x : data)
		if (x > ret)
			ret = x;
	return ret;
}


template <typename T, size_t ROWS, size_t COLS>
template <typename Functor>
MatrixBase<T, ROWS, COLS>& MatrixBase<T, ROWS, COLS>::apply(Functor f)
{
	for (auto it = begin(); it != end(); it++)
		f(*it);
	return *this;
}

template <typename T, size_t ROWS, size_t COLS>
template<typename M, typename Functor>
inline MatrixBase<T, ROWS, COLS>& MatrixBase<T, ROWS, COLS>::apply(const M& m, Functor f)
{
	assert(rows == m.rows && cols == m.cols);
	for (auto i = begin(), j = m.begin(); i != end(); i++, j++)
		f(*i, *j);
	return *this;
}

// Scalar assignment
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>&
MatrixBase<T, ROWS, COLS>::operator=(const T& x)
{
	return apply([&](T& y) {y = x;});
}

// Scalar addition
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>&
MatrixBase<T, ROWS, COLS>::operator+=(const T& x)
{
	return apply([&](T& y) {y += x;});
}

// Scalar subtraction
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>&
MatrixBase<T, ROWS, COLS>::operator-=(const T& x)
{
	return apply([&](T& y) {y -= x;});
}

// Scalar multiplication
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>&
MatrixBase<T, ROWS, COLS>::operator*=(const T& x)
{
	return apply([&](T& y) {y *= x;});
}

// Scalar division
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>&
MatrixBase<T, ROWS, COLS>::operator/=(const T& x)
{
	return apply([&](T& y) {y /= x;});
}

// Scalar remainder
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>&
MatrixBase<T, ROWS, COLS>::operator%=(const T& x)
{
	return apply([&](T& y) {y %= x;});
}

// Matrix addition
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>& MatrixBase<T, ROWS, COLS>::operator+=(const MatrixBase<T, ROWS, COLS>& m)
{
	return apply(m, [&](T& t, const T& u) {t += u;});
}

// Matrix subtraction
template<typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>& MatrixBase<T, ROWS, COLS>::operator-=(const MatrixBase<T, ROWS, COLS>& m)
{
	return apply(m, [&](T& t, const T& u) {t -= u;});
}

template<typename T, size_t ROWS, size_t COLS>
template<typename rhsT, size_t rhsCOLS>
MatrixBase<T, ROWS, rhsCOLS> MatrixBase<T, ROWS, COLS>::operator*(const MatrixBase<rhsT, COLS, rhsCOLS>& rhs) const
{
	MatrixBase<T, ROWS, rhsCOLS> ret;
	for (int h = 0; h < rhsCOLS; h++) {
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				ret.at(i, h) += at(i, j) * rhs.at(j, h);
			}
		}
	}
	return ret;
}

template<typename T, size_t ROWS, size_t COLS>
bool MatrixBase<T, ROWS, COLS>::lup(MatrixBase<T, ROWS, COLS> &L, MatrixBase<T, ROWS, COLS> &U, MatrixBase<T, ROWS, COLS> &P) const
{
	assert(ROWS == COLS);
	int K, k, i, j;

	L.clear();
	P =  MatrixBase<T, ROWS, COLS>::identity();
	U = *this;
	for (k = 0; k < ROWS; k++) {
		T p = 0;
		K = k;
		for (i = k; i < ROWS; i++) {
			T u = abs(U.at(i, k));
			if (u > p) {
				p = u;
				K = i;
			}
		}
		if (!p)
			return false;
		P.swap_rows(k, K);
		U.swap_rows(k, K);
		L.swap_rows(k, K);
		for (i = k + 1; i < ROWS; i++) {
			L.at(i, k) = U.at(i, k) / U.at(k, k);
			for (j = 0; j < COLS; j++)
				U.at(i, j) -= L.at(i, k) * U.at(k, j);
		}
	}
	for (i = 0; i < ROWS; i++)
		L.at(i, i) = static_cast<T>(1);
	return true;
}

template<typename T, size_t ROWS, size_t COLS>
MatrixBase<T, ROWS, COLS> MatrixBase<T, ROWS, COLS>::inverse() const
{
	assert(ROWS == COLS);
	MatrixBase<T, ROWS, COLS> ret;
	MatrixBase<T, ROWS, 1> In;
	MatrixBase<T, ROWS, COLS> L, U, P;

	if (!lup(L, U, P))
		return ret;
	for (int i = 0; i < ROWS; i++) {
		In.at(i, 0) = 1;
		ret.set_column(i, lup_solve(L, U, P, In));
		In.at(i, 0) = 0;
	}
	return ret;
}

template<typename T, size_t ROWS, size_t COLS>
MatrixMN<T, ROWS, COLS>& MatrixMN<T, ROWS, COLS>::operator=(const T& x)
{
	return static_cast<MatrixMN<T, ROWS, COLS>&>(base::operator=(x));
}

template<typename T, size_t ROWS, size_t COLS>
MatrixMN<T, ROWS, COLS> MatrixMN<T, ROWS, COLS>::operator-(const MatrixMN<T, ROWS, COLS>& m) const
{
	return static_cast<MatrixMN<T, ROWS, COLS>>(base::operator-(m));
}

template<typename T, size_t ROWS, size_t COLS>
MatrixMN<T, ROWS, COLS> MatrixMN<T, ROWS, COLS>::operator+(const MatrixMN<T, ROWS, COLS>& m) const
{
	return static_cast<MatrixMN<T, ROWS, COLS>>(base::operator+(m));
}


template<typename T, size_t ROWS>
T MatrixMN<T, ROWS, 1>::dot(const MatrixMN<T, ROWS, 1>& v)
{
	return std::inner_product(base::begin(), base::end(), v.begin(), static_cast<T>(0));
}

template<typename T, size_t ROWS>
MatrixMN<T, 3, 1> MatrixMN<T, ROWS, 1>::cross(const MatrixMN<T, 3, 1>& v) const
{
	return MatrixMN<T, 3, 1>(
			at(1) * v.at(2) - v.at(1) * at(2),
			at(2) * v.at(0) - v.at(2) * at(0),
			at(0) * v.at(1) - v.at(0) * at(1));
}

template<typename T, size_t ROWS>
MatrixMN<T, 3, 1> MatrixMN<T, ROWS, 1>::projection(const MatrixMN<T, 3, 1>& u) const
{
	return (*this) * dot(u) / length_squared();
}

template<typename T, size_t ROWS>
MatrixMN<T, 3, 1> MatrixMN<T, ROWS, 1>::perpendicular(const MatrixMN<T, 3, 1>& u) const
{
	return u - projection(u);
}

/*
 * qn denotes the normal vector to a plane
 */
template<typename T, size_t ROWS>
MatrixMN<T, 3, 1> MatrixMN<T, ROWS, 1>::parallel(const MatrixMN<T, 3, 1>& v) const
{
	Quaternion<T> qn(0, at(0), at(1), at(2));
	Quaternion<T> qv(0, v.at(0), v.at(1), v.at(2));
	Quaternion<T> qr = (qv + qn * qv * qn) * 1.0 / 2.0;
	return MatrixMN<T, 3, 1>(qr.x, qr.y, qr.z);
}

/*
 * qn denotes the normal vector to a plane
 */
template<typename T, size_t ROWS>
MatrixMN<T, 3, 1> MatrixMN<T, ROWS, 1>::perpendicular2(const MatrixMN<T, 3, 1>& v) const
{
	Quaternion<T> qn(0, at(0), at(1), at(2));
	Quaternion<T> qv(0, v.at(0), v.at(1), v.at(2));
	Quaternion<T> qr = (qv - qn * qv * qn) * 1.0 / 2.0;
	return MatrixMN<T, 3, 1>(qr.x, qr.y, qr.z);
}

/*
 * qn denotes the normal vector to a plane
 */
template<typename T, size_t ROWS>
MatrixMN<T, 3, 1> MatrixMN<T, ROWS, 1>::reflection(const MatrixMN<T, 3, 1>& v) const
{
	Quaternion<T> qn(0, at(0), at(1), at(2));
	Quaternion<T> qv(0, v.at(0), v.at(1), v.at(2));
	Quaternion<T> qr = (qn * qv * qn);
	return MatrixMN<T, 3, 1>(qr.x, qr.y, qr.z);
}

template<typename T, size_t ROWS>
MatrixMN<T, ROWS, 1> MatrixMN<T, ROWS, 1>::operator-(const MatrixMN<T, ROWS, 1>& m) const
{
	return static_cast<MatrixMN<T, ROWS, 1>>(base::operator-(m));
}

template<typename T, size_t ROWS>
MatrixMN<T, ROWS, 1> MatrixMN<T, ROWS, 1>::operator+(const MatrixMN<T, ROWS, 1>& m) const
{
	return static_cast<MatrixMN<T, ROWS, 1>>(base::operator+(m));
}

template<typename T, size_t ROWS>
MatrixMN<T, ROWS, 1>& MatrixMN<T, ROWS, 1>::operator=(const MatrixMN<T, ROWS, 1>& m)
{
	return static_cast<MatrixMN<T, ROWS, 1>&>(base::operator=(m));
}

template<typename T, size_t ROWS>
MatrixMN<T, ROWS, 1>& MatrixMN<T, ROWS, 1>::operator=(const T& x)
{
	return static_cast<MatrixMN<T, ROWS, 1>&>(base::operator=(x));
}

template<typename T, size_t ROWS>
T MatrixMN<T, ROWS, 1>::length_squared() const
{
	return std::inner_product(base::begin(), base::end(), base::begin(), static_cast<T>(0));
}

template<typename T, size_t ROWS>
T MatrixMN<T, ROWS, 1>::length() const
{
	return std::sqrt(length_squared());
}

template<typename T, size_t ROWS>
MatrixMN<T, ROWS, 1> MatrixMN<T, ROWS, 1>::normalize() const
{
	return (*this / length());
}

template<typename T, size_t ROWS>
T& MatrixMN<T, ROWS, 1>::at(size_t row)
{
	return base::at(row, 0);
}

template<typename T, size_t ROWS>
const T& MatrixMN<T, ROWS, 1>::at(size_t row) const
{
	return base::at(row, 0);
}

template<typename T, size_t ROWS>
T& MatrixMN<T, ROWS, 1>::operator[](size_t row)
{
	return at(row);
}

template<typename T, size_t ROWS>
const T& MatrixMN<T, ROWS, 1>::operator[](size_t row) const
{
	return at(row);
}

template<typename T, size_t ROWS>
T& MatrixMN<T, ROWS, 1>::x()
{
	assert(base::rows > 0);
	return at(0);
}

template<typename T, size_t ROWS>
const T& MatrixMN<T, ROWS, 1>::x() const
{
	assert(base::rows > 0);
	return at(0);
}

template<typename T, size_t ROWS>
T& MatrixMN<T, ROWS, 1>::y()
{
	assert(base::rows > 1);
	return at(1);
}

template<typename T, size_t ROWS>
const T& MatrixMN<T, ROWS, 1>::y() const
{
	assert(base::rows > 1);
	return at(1);
}

template<typename T, size_t ROWS>
T& MatrixMN<T, ROWS, 1>::z()
{
	assert(base::rows > 2);
	return at(2);
}

template<typename T, size_t ROWS>
const T& MatrixMN<T, ROWS, 1>::z() const
{
	assert(base::rows > 2);
	return at(2);
}

template<typename T, size_t ROWS>
T& MatrixMN<T, ROWS, 1>::w()
{
	assert(base::rows > 3);
	return at(3);
}

template<typename T, size_t ROWS>
const T& MatrixMN<T, ROWS, 1>::w() const
{
	assert(base::rows > 3);
	return at(3);
}

template<typename T>
MatrixMN<T, 4, 4> create_rotation_matrix(T xrad, T yrad, T zrad)
{
	float cx = cos(xrad);
	float sx = sin(xrad);
	float cy = cos(yrad);
	float sy = sin(yrad);
	float cz = cos(zrad);
	float sz = sin(zrad);
	MatrixMN<T, 4, 4> mx{ 1,   0,   0,  0,
						  0,  cx, -sx,  0,
						  0,  sx,  cx,  0,
						  0,   0,   0,  1};
	MatrixMN<T, 4, 4> my{ cy,   0,  sy,  0,
						   0,   1,   0,  0,
						 -sy,   0,  cy,  0,
						   0,   0,   0,  1};
	MatrixMN<T, 4, 4> mz{ cz, -sz,  0,  0,
						  sz,  cz,  0,  0,
						   0,   0,  1,  0,
						   0,   0,  0,  1};
	return mx * my * mz;
}

template<typename T>
MatrixMN<T, 4, 4> create_translation_matrix(T x, T y, T z)
{
	MatrixMN<T, 4, 4> ret{	1,   0,  0,  x,
							0,   1,  0,  y,
							0,   0,  1,  z,
							0,   0,  0,  1};
	return ret;
}

template<typename T>
MatrixMN<T, 4, 4> create_scale_matrix(T x, T y, T z)
{
	MatrixMN<T, 4, 4> ret{	x,   0,  0,  0,
							0,   y,  0,  0,
							0,   0,  z,  0,
							0,   0,  0,  1};
	return ret;
}

template<typename T>
MatrixMN<T, 4, 4> create_frustum_matrix(T left, T right, T bottom, T top, T zNear, T zFar)
{
	MatrixMN<T, 4, 4> ret{

		2 * zNear/(right-left),	0,						(right+left)/(right-left),	0,
		0,						2*zNear/(top-bottom),	(top+bottom)/(top-bottom),	0,
		0,						0,						-(zFar+zNear)/(zFar-zNear),	-(2*zFar*zNear)/(zFar-zNear),
		0,						0,						-1,							0
	};
	return ret;
}

template<typename T>
MatrixMN<T, 4, 4> create_ortho_matrix(T left, T right, T bottom, T top, T zNear, T zFar)
{
	MatrixMN<T, 4, 4> ret
	{
		2/(right-left),			0,						0,							(right+left)/(right-left),
		0,						2/(top-bottom),			0,							(top+bottom)/(top-bottom),
		0,						0,						-2/(zFar-zNear),			-(zFar+zNear)/(zFar-zNear),
		0,						0,						0,							1
	};
	return ret;
}

template<typename T>
MatrixMN<T, 4, 4> create_look_at(
		const MatrixMN<T, 3, 1>& eyePos,
		const MatrixMN<T, 3, 1>& centerPos,
		const MatrixMN<T, 3, 1>& upDir)
{
	MatrixMN<T, 3, 1> forward, side, up(upDir);
	MatrixMN<T, 4, 4> m = MatrixMN<T, 4, 4>::identity();

	forward = (centerPos - eyePos).normalize();

	// Side = forward x up
	side = forward.cross(up).normalize();

	// Recompute up as: up = side x forward
	up = side.cross(forward).normalize();

	m.at(0, 0) = side.at(0,0);
	m.at(0, 1) = side.at(1,0);
	m.at(0, 2) = side.at(2,0);

	m.at(1, 0) = up.at(0,0);
	m.at(1, 1) = up.at(1,0);
	m.at(1, 2) = up.at(2,0);

	m.at(2, 0) = -forward.at(0,0);
	m.at(2, 1) = -forward.at(1,0);
	m.at(2, 2) = -forward.at(2,0);

	return (m * create_translation_matrix(-eyePos.at(0,0), -eyePos.at(1,0), -eyePos.at(2,0)));
}


#ifdef D3MATH_NAMESPACE
}
#endif

#endif

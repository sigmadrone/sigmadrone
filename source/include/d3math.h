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


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef DEG2RAD
#define DEG2RAD(__d__) (((__d__) * M_PI) / 180.0)
#define RAD2DEG(__r__) (((__r__) / M_PI) * 180.0)
#endif

#ifdef MATRIX_NAMESPACE
namespace MATRIX_NAMESPACE
{
#endif

#ifndef COUNT_OF
#define COUNT_OF(__x__) (sizeof(__x__)/sizeof(__x__[0]))
#endif

const double EPSILON = 4.37114e-05;

template <typename T = float, size_t ROWS = 1, size_t COLS = 1>
class MatrixBase {
public:
	using array_type = std::array<T, ROWS * COLS>;
	using value_type = typename array_type::value_type;
    using iterator       = typename array_type::iterator;
    using const_iterator = typename array_type::const_iterator;

public:
	std::array<T, ROWS * COLS> data;
	static constexpr size_t rows = ROWS;
	static constexpr size_t cols = COLS;

	MatrixBase();
	MatrixBase(const std::initializer_list<std::initializer_list<T>>& list);
	MatrixBase(const std::initializer_list<T>& list);

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

	void set_row(size_t m, const MatrixBase<T, 1, COLS>& r);
	void set_column(size_t n, const MatrixBase<T, ROWS, 1>& c);
	MatrixBase<T, 1, COLS> row(size_t m) const;
	MatrixBase<T, ROWS, 1> column(size_t n) const;

    // Scalar arithmetic
	MatrixBase& operator=(const T& x);
	MatrixBase& operator+=(const T& x);
	MatrixBase& operator-=(const T& x);
	MatrixBase& operator*=(const T& x);
	MatrixBase& operator/=(const T& x);
	MatrixBase& operator%=(const T& x);

	// Matrix arithmetic
	MatrixBase& operator-=(const MatrixBase& m);
	MatrixBase& operator+=(const MatrixBase& m);

	iterator begin() 						{ return data.begin(); }
	iterator end()   						{ return data.end(); }

	const_iterator begin() const 			{ return data.begin(); }
	const_iterator end() const   			{ return data.end(); }


	template <typename TT, size_t R, size_t C>
	friend std::ostream& operator<<(std::ostream& os, const MatrixBase<TT, R, C>& m);

	template<size_t R = ROWS, size_t C = COLS>
	static typename std::enable_if<R == C, MatrixBase<T, ROWS, COLS>>::type identity();

protected:
	template <typename Functor>
	MatrixBase& apply(Functor f);

	template <typename M, typename Functor>
	MatrixBase& apply(const M& m, Functor f);


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

	MatrixMN& operator=(const T& x)			{ return static_cast<MatrixMN&>(base::operator=(x)); }
};


template <typename T, size_t ROWS>
class MatrixMN<T, ROWS, 1> : public MatrixBase<T, ROWS, 1>
{
public:
	T &x, &y, &z, &w;

public:
	using base = MatrixBase<T, ROWS, 1>;

public:
	MatrixMN() : MatrixBase<T, ROWS, 1>(), x{base::data[0]}, y{base::data[1]}, z{base::data[2]}, w{base::data[3]} {} ;
	MatrixMN(const std::initializer_list<std::initializer_list<T>>& list) : MatrixBase<T, ROWS, 1>(list), x{base::data[0]}, y{base::data[1]}, z{base::data[2]}, w{base::data[3]} {}
	MatrixMN(const std::initializer_list<T>& list) : MatrixBase<T, ROWS, 1>(list), x{base::data[0]}, y{base::data[1]}, z{base::data[2]}, w{base::data[3]} {}
	MatrixMN(const MatrixBase<T, ROWS, 1>& m) : MatrixBase<T, ROWS, 1>(m), x{base::data[0]}, y{base::data[1]}, z{base::data[2]}, w{base::data[3]} {} ;

	MatrixMN& operator=(const T& x)			{ return static_cast<MatrixMN&>(base::operator=(x)); }


	T length_squared() const
	{
		T ret = static_cast<T>(0);
		for (int i = 0; i < ROWS; i++)
			ret += base::at(i, 0) * base::at(i, 0);
		return ret;
	}

	T length() const						{ return std::sqrt(length_squared()); }
	T& at(size_t row)						{ return base::at(row, 0); }
	const T& at(size_t row) const			{ return base::at(row, 0); }

};




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
template <size_t R, size_t C>
typename std::enable_if<R == C, MatrixBase<T, ROWS, COLS>>::type MatrixBase<T, ROWS, COLS>::identity()
{
	MatrixBase<T, ROWS, COLS> ret;
	for (size_t i = 0; i < ROWS; i++)
		for (size_t j = 0; j < COLS; j++)
			ret.at(i, j) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
	return ret;
}

template <typename TT, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const MatrixBase<TT, R, C>& m)
{
	os << m.to_string();
	return os;
}


template <typename T, size_t ROWS, size_t COLS>
inline MatrixBase<T, ROWS, COLS>::MatrixBase() {
	for (auto& i : data)
		i = static_cast<T>(0);
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
inline std::string MatrixBase<T, ROWS, COLS>::to_string() const
{
	std::stringstream oss;
	oss.setf(std::ios::fixed, std::ios::floatfield);
	oss.precision(2);
	for (size_t i = 0; i < ROWS; i++) {
		for (size_t j = 0; j < COLS; j++) {
			oss.width(7);
			oss << at(i, j) << " ";
		}
		oss << std::endl;
	}
	return oss.str();
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
template <typename Functor>
MatrixBase<T, ROWS, COLS>& MatrixBase<T, ROWS, COLS>::apply(Functor f)
{
	for (auto& x : data)
		f(x);
	return *this;
}

template <typename T, size_t ROWS, size_t COLS>
template<typename M, typename Functor>
inline MatrixBase<T, ROWS, COLS>& MatrixBase<T, ROWS, COLS>::apply(const M& m, Functor f)
{
	assert(rows == m.rows && cols == m.cols);
	auto i = begin();
	auto j = m.begin();
	while (i != end()) {
		f(*i, *j);
		++i;
		++j;
	}
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


#ifdef MATRIX_NAMESPACE
}
#endif

#endif

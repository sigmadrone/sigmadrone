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

#if 0
template <typename T, size_t ROWS, size_t COLS>
class MatrixMN {
public:
	std::array<T, ROWS * COLS> data;

};

#endif

template <typename T, size_t ROWS, size_t COLS>
class MatrixMN
{
public:
	using array_type = std::array<T, ROWS * COLS>;
	using value_type = typename array_type::value_type;
    using iterator       = typename array_type::iterator;
    using const_iterator = typename array_type::const_iterator;

public:
	std::array<T, ROWS * COLS> data;

public:
	MatrixMN();
	MatrixMN(const std::initializer_list<std::initializer_list<T>>& list);
	MatrixMN(const std::initializer_list<T>& list);

	T& at(size_t row, size_t col);
	const T& at(size_t row, size_t col) const;
	std::string toString() const;

	template <typename TT, size_t R, size_t C>
	friend std::ostream& operator<< (std::ostream& os, const MatrixMN<TT, R, C>& m);

	static MatrixMN<T, ROWS, COLS> identity();
	void clear();
	MatrixMN<T, 1, COLS> row(int m) const;
	void setRow(int m, const MatrixMN<T, 1, COLS>& r);
	MatrixMN<T, ROWS, 1> column(int n) const;
	void setColumn(int n, const MatrixMN<T, ROWS, 1>& c);

};



template <typename T, size_t ROWS, size_t COLS>
inline MatrixMN<T, ROWS, COLS>::MatrixMN() {
	for (auto& i : data)
		i = static_cast<T>(0);
}

template <typename T, size_t ROWS, size_t COLS>
inline MatrixMN<T, ROWS, COLS>::MatrixMN(const std::initializer_list<std::initializer_list<T>>& list) {
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
inline MatrixMN<T, ROWS, COLS>::MatrixMN(const std::initializer_list<T>& list) {
	assert(list.size() == ROWS * COLS);
	iterator it = data.begin();
	for (auto elem : list) {
		*it++ = elem;
	}
}

template <typename T, size_t ROWS, size_t COLS>
inline T& MatrixMN<T, ROWS, COLS>::at(size_t row, size_t col)
{
	return data[row * COLS + col];
}

template <typename T, size_t ROWS, size_t COLS>
inline const T& MatrixMN<T, ROWS, COLS>::at(size_t row, size_t col) const
{
	return data[row * COLS + col];
}

template <typename T, size_t ROWS, size_t COLS>
inline MatrixMN<T, ROWS, COLS> MatrixMN<T, ROWS, COLS>::identity()
{
	assert(ROWS == COLS);
	MatrixMN<T, ROWS, COLS> ret;
	for (size_t i = 0; i < ROWS; i++)
		for (size_t j = 0; j < COLS; j++)
			ret.at(i, j) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixMN<T, ROWS, COLS>::clear()
{
	for (size_t i = 0; i < ROWS; i++)
		for (size_t j = 0; j < COLS; j++)
			at(i, j) = static_cast<T>(0);
}

template <typename T, size_t ROWS, size_t COLS>
MatrixMN<T, 1, COLS> MatrixMN<T, ROWS, COLS>::row(int m) const
{
	MatrixMN<T, 1, COLS> ret;
	for (int i = 0; i < COLS; i++)
		ret.at(0, i) = at(m, i);
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixMN<T, ROWS, COLS>::setRow(int m, const MatrixMN<T, 1, COLS>& r)
{
	for (int i = 0; i < COLS; i++)
		at(m, i) = r.at(0, i);
}

template <typename T, size_t ROWS, size_t COLS>
MatrixMN<T, ROWS, 1> MatrixMN<T, ROWS, COLS>::column(int n) const
{
	MatrixMN<T, ROWS, 1> ret;
	for (int i = 0; i < ROWS; i++)
		ret.at(i, 0) = at(i, n);
	return ret;
}

template <typename T, size_t ROWS, size_t COLS>
void MatrixMN<T, ROWS, COLS>::setColumn(int n, const MatrixMN<T, ROWS, 1>& c)
{
	for (int i = 0; i < ROWS; i++)
		at(i, n) = c.at(i, 0);
}


template <typename T, size_t ROWS, size_t COLS>
inline std::string MatrixMN<T, ROWS, COLS>::toString() const
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
std::ostream& operator<<(std::ostream& os, const MatrixMN<T, ROWS, COLS>& m)
{
	os << m.toString();
	return os;
}

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

#ifdef MATRIX_NAMESPACE
}
#endif

#endif

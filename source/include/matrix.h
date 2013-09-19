/*
 * Copyright (C) 2013 Martin Stoilov
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <sstream>
#include <cassert>
#include <cmath>

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

const double EPSILON = 4.37114e-05;

template <typename T, int M, int N>
class MatrixMN
{
public:
	/*
	 * Data is stored in column major order
	 */
	T data[M * N];

public:
	/*
	 * Data must be provided in row major order,
	 * but it will be converted to row major order.
	 * If the data is already in column major order
	 * set transposed to true.
	 */
	MatrixMN(T *a, bool transposed = false)
	{
		if (transposed) {
		for (int i = 0; i < M * N; i++)
			data[i] = a[i];
		} else {
			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
					at(i, j) = a[i * M + j];
		}
	}

	MatrixMN(T a0 = 0, T a1 = 0, T a2 = 0, T a3 = 0, T a4 = 0, T a5 = 0, T a6 = 0, T a7 = 0,
			T a8 = 0, T a9 = 0, T a10 = 0, T a11 = 0, T a12 = 0, T a13 = 0, T a14 = 0, T a15 = 0, T a16 = 0,
			T a17 = 0, T a18 = 0, T a19 = 0, T a20 = 0, T a21 = 0, T a22 = 0, T a23 = 0, T a24 = 0)
	{
		T temp[25];
		temp[0] = a0; temp[1] = a1; temp[2] = a2; temp[3] = a3; temp[4] = a4;
		temp[5] = a5; temp[6] = a6; temp[7] = a7; temp[8] = a8; temp[9] = a9;
		temp[10] = a10; temp[11] = a11; temp[12] = a12;
		temp[13] = a13; temp[14] = a14; temp[15] = a15;
		temp[16] = a16; temp[17] = a17; temp[18] = a18;
		temp[19] = a19; temp[20] = a20; temp[21] = a21;
		temp[22] = a22; temp[23] = a23; temp[24] = a24;

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				at(i, j) = ((i * N + j) < (int)(sizeof(temp)/sizeof(temp[0]))) ? temp[i * N + j] : static_cast<T>(0);
	}

	template<typename From, int FromM, int FromN>
	MatrixMN(const MatrixMN<From, FromM, FromN>& src)
	{
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				at(i, j) = (i < FromM && j < FromN) ? src.at(i, j) : static_cast<T>(0);
			}
		}
	}

	T& at(int row, int col)
	{
		return data[col * M + row];
	}

	const T& at(int row, int col) const
	{
		return data[col * M + row];
	}

	void clear()
	{
		for (int i = 0; i < N * M; i++)
			data[i] = static_cast<T>(0);
	}

	void identity()
	{
		assert(M == N);
		for (int i = 0; i < M * M; i++)
			data[i] = (i % (M + 1)) ? static_cast<T>(0) : static_cast<T>(1);
	}

	template<typename From, int FromM, int FromN>
	MatrixMN<T, M, N>& operator=(const MatrixMN<From, FromM, FromN>& src)
	{
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				at(i, j) = (i < FromM && j < FromN) ? src.at(i, j) : static_cast<T>(0);
			}
		}
		return *this;
	}

	MatrixMN<T, 1, N> row(int m) const
	{
		MatrixMN<T, 1, N> ret;
		for (int i = 0; i < N; i++)
			ret.at(0, i) = at(m, i);
		return ret;
	}

	void setRow(int m, const MatrixMN<T, 1, N>& r)
	{
		for (int i = 0; i < N; i++)
			at(m, i) = r.at(0, i);
	}

	MatrixMN<T, M, 1> column(int n) const
	{
		MatrixMN<T, M, 1> ret;
		for (int i = 0; i < M; i++)
			ret.at(i, 0) = at(i, n);
		return ret;
	}

	void setColumn(int n, const MatrixMN<T, M, 1>& c)
	{
		for (int i = 0; i < M; i++)
			at(i, n) = c.at(i, 0);
	}

	void swapRows(int m, int n)
	{
		T temp;
		if (m == n)
			return;
		for (int i = 0; i < N; i++) {
			temp = at(m, i);
			at(m, i) = at(n, i);
			at(n, i) = temp;
		}
	}

	void swapColumns(int m, int n)
	{
		T temp;
		if (m == n)
			return;
		for (int i = 0; i < M; i++) {
			temp = at(i, m);
			at(i, m) = at(i, n);
			at(i, n) = temp;
		}
	}

	MatrixMN<T, M, N> operator+(const MatrixMN<T, M, N>& rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] + rhs.data[i];
		return ret;
	}

	MatrixMN<T, M, N> operator+(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] + rhs;
		return ret;
	}

	MatrixMN<T, M, N> operator-(const MatrixMN<T, M, N>& rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] - rhs.data[i];
		return ret;
	}

	MatrixMN<T, M, N> operator-(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] - rhs;
		return ret;
	}

	MatrixMN<T, M, N> operator/(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] / rhs;
		return ret;
	}

	MatrixMN<T, M, N> operator*(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] * rhs;
		return ret;
	}

	template<typename rhsT, int rhsN>
	MatrixMN<T, M, rhsN> operator*(const MatrixMN<rhsT, N, rhsN>& rhs) const
	{
		MatrixMN<T, M, rhsN> ret;
		for (int h = 0; h < rhsN; h++) {
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					ret.at(i, h) += at(i, j) * rhs.at(j, h);
				}
			}
		}
		return ret;
	}

	MatrixMN<T, N, M> transpose() const
	{
		MatrixMN<T, N, M> ret;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				ret.at(i, j) = at(j, i);
		return ret;
	}

	bool lup(MatrixMN<T, M, N> &L, MatrixMN<T, M, N> &U, MatrixMN<T, M, N> &P) const
	{
		assert(M == N);
		int K, k, i, j;

		L.clear();
		P.identity();
		U = *this;
		for (k = 0; k < M; k++) {
			T p = 0;
			K = k;
			for (i = k; i < M; i++) {
				if (abs(U.at(i, k)) > p) {
					p = abs(U.at(i, k));
					K = i;
				}
			}
			if (!p)
				return false;
			P.swapRows(k, K);
			U.swapRows(k, K);
			L.swapRows(k, K);
			for (i = k + 1; i < M; i++) {
				L.at(i, k) = U.at(i, k)/U.at(k, k);
				for (j = 0; j < N; j++)
					U.at(i, j) -= L.at(i, k) * U.at(k, j);
			}
		}
		for (i = 0; i < M; i++)
			L.at(i, i) = static_cast<T>(1);
		return true;
	}

	MatrixMN<T, M, 1> lupSolve(
			const MatrixMN<T, M, N> &L,
			const MatrixMN<T, M, N> &U,
			const MatrixMN<T, M, N> &P,
			const MatrixMN<T, M, 1> &B) const
	{
		MatrixMN<T, M, 1> X;
		MatrixMN<T, M, 1> Y = P * B;
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < i; j++)
				Y.at(i, 0) -= Y.at(j, 0)*L.at(i, j);
		}
		X = Y;
		for (int i = M - 1; i >= 0; i--) {
			for (int j = i + 1; j < M; j++)
				X.at(i, 0) -= X.at(j, 0)*U.at(i, j);
			X.at(i, 0) /= U.at(i,i);
		}
		return X;
	}

	MatrixMN<T, M, N> inverse() const
	{
		assert(M == N);
		MatrixMN<T, M, N> ret;
		MatrixMN<T, M, 1> In;
		MatrixMN<T, M, N> L, U, P;

		if (!lup(L, U, P))
			return ret;
		for (int i = 0; i < M; i++) {
			In.at(i, 0) = 1;
			ret.setColumn(i, lupSolve(L, U, P, In));
			In.at(i, 0) = 0;
		}
		return ret;
	}

	std::string toString() const
	{
		std::stringstream oss;
		oss.setf(std::ios::fixed, std::ios::floatfield);
		oss.precision(2);
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				oss.width(7);
				oss << at(i, j) << " ";
			}
			oss << std::endl;
		}
		return oss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const MatrixMN<T, M, N>& m)
	{
		os << m.toString();
		return os;
	}

	static MatrixMN<T, 4, 4> createRotationMatrix(T xrad, T yrad, T zrad)
	{
		float cx = cos(xrad);
		float sx = sin(xrad);
		float cy = cos(yrad);
		float sy = sin(yrad);
		float cz = cos(zrad);
		float sz = sin(zrad);
		MatrixMN<T, 4, 4> mx( 1,   0,   0,  0,
							  0,  cx, -sx,  0,
							  0,  sx,  cx,  0,
							  0,   0,   0,  1);
		MatrixMN<T, 4, 4> my( cy,   0,  sy,  0,
							   0,   1,   0,  0,
							 -sy,   0,  cy,  0,
							   0,   0,   0,  1);
		MatrixMN<T, 4, 4> mz( cz, -sz,  0,  0,
							  sz,  cz,  0,  0,
							   0,   0,  1,  0,
							   0,   0,  0,  1);
		return mx * my * mz;
	}

	static MatrixMN<T, 4, 4> createTranslationMatrix(T x, T y, T z)
	{
		MatrixMN<T, 4, 4> ret(	1,   0,  0,  x,
								0,   1,  0,  y,
								0,   0,  1,  z,
								0,   0,  0,  1);
		return ret;
	}

	static MatrixMN<T, 4, 4> createScaleMatrix(T x, T y, T z)
	{
		MatrixMN<T, 4, 4> ret(	x,   0,  0,  0,
								0,   y,  0,  0,
								0,   0,  z,  0,
								0,   0,  0,  1);
		return ret;
	}

	static MatrixMN<T, M, M> createIdentityMatrix()
	{
		MatrixMN<T, M, M> ret;
		ret.identity();
		return ret;
	}

	static MatrixMN<T, 4, 4> createFrustumMatrix(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		MatrixMN<T, 4, 4> ret
		(
			2 * zNear/(right-left),	0,						(right+left)/(right-left),	0,
			0,						2*zNear/(top-bottom),	(top+bottom)/(top-bottom),	0,
			0,						0,						-(zFar+zNear)/(zFar-zNear),	-(2*zFar*zNear)/(zFar-zNear),
			0,						0,						-1,							0
		);
		return ret;
	}

	static MatrixMN<T, 4, 4> createOrthoMatrix(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		MatrixMN<T, 4, 4> ret
		(
			2/(right-left),			0,						0,							(right+left)/(right-left),
			0,						2/(top-bottom),			0,							(top+bottom)/(top-bottom),
			0,						0,						-2/(zFar-zNear),			-(zFar+zNear)/(zFar-zNear),
			0,						0,						0,							1
		);
		return ret;
	}

	static MatrixMN<T, 4, 4> createLookAt(
			const MatrixMN<T, 3, 1>& eyePos,
			const MatrixMN<T, 3, 1>& centerPos,
			const MatrixMN<T, 3, 1>& upDir)
	{
		MatrixMN<T, 3, 1> forward, side, up(upDir);
		MatrixMN<T, 4, 4> m;

		m.identity();
		forward = (centerPos - eyePos).normalize();

		// Side = forward x up
		side = MatrixMN<T, 3, 1>::cross(forward, up).normalize();

		// Recompute up as: up = side x forward
		up = MatrixMN<T, 3, 1>::cross(side, forward);

		m.at(0, 0) = side.at(0,0);
		m.at(0, 1) = side.at(1,0);
		m.at(0, 2) = side.at(2,0);

		m.at(1, 0) = up.at(0,0);
		m.at(1, 1) = up.at(1,0);
		m.at(1, 2) = up.at(2,0);

		m.at(2, 0) = -forward.at(0,0);
		m.at(2, 1) = -forward.at(1,0);
		m.at(2, 2) = -forward.at(2,0);

		return (m * MatrixMN<T, 4, 4>::createTranslationMatrix(-eyePos.at(0,0), -eyePos.at(1,0), -eyePos.at(2,0)));
	}

private:
	T abs(T a) const
	{
		return (a < 0) ? -a : a;
	}
};


template<typename T, int M>
class MatrixMN<T, M, 1>
{
public:
	static const int N = 1;

	/*
	 * Data is stored in column major order
	 */
	T data[M * N];

public:
	/*
	 * Data must be provided in row major order,
	 * but it will be converted to row major order.
	 * If the data is already in column major order
	 * set transposed to true.
	 */
	MatrixMN(T *a, bool transposed = false)
	{
		if (transposed) {
		for (int i = 0; i < M * N; i++)
			data[i] = a[i];
		} else {
			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
					at(i, j) = a[i * M + j];
		}
	}

	MatrixMN(T a0 = 0, T a1 = 0, T a2 = 0, T a3 = 0, T a4 = 0, T a5 = 0, T a6 = 0, T a7 = 0, T a8 = 0,
			T a9 = 0, T a10 = 0, T a11 = 0, T a12 = 0, T a13 = 0, T a14 = 0, T a15 = 0,	T a16 = 0,
			T a17 = 0, T a18 = 0, T a19 = 0, T a20 = 0, T a21 = 0, T a22 = 0, T a23 = 0, T a24 = 0)
	{
		T temp[25];
		temp[0] = a0; temp[1] = a1; temp[2] = a2; temp[3] = a3; temp[4] = a4;
		temp[5] = a5; temp[6] = a6; temp[7] = a7; temp[8] = a8; temp[9] = a9;
		temp[10] = a10; temp[11] = a11; temp[12] = a12;
		temp[13] = a13; temp[14] = a14; temp[15] = a15;
		temp[16] = a16; temp[17] = a17; temp[18] = a18;
		temp[19] = a19; temp[20] = a20; temp[21] = a21;
		temp[22] = a22; temp[23] = a23; temp[24] = a24;

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				at(i, j) = ((i * N + j) < (int)(sizeof(temp)/sizeof(temp[0]))) ? temp[i * N + j] : static_cast<T>(0);
	}

	template<typename From, int FromM, int FromN>
	MatrixMN(const MatrixMN<From, FromM, FromN>& src)
	{
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				at(i, j) = (i < FromM && j < FromN) ? src.at(i, j) : static_cast<T>(0);
			}
		}
	}

	T& at(int row, int col)
	{
		return data[col * M + row];
	}

	const T& at(int row, int col) const
	{
		return data[col * M + row];
	}

	void clear()
	{
		for (int i = 0; i < N * M; i++)
			data[i] = static_cast<T>(0);
	}

	template<typename From, int FromM, int FromN>
	MatrixMN<T, M, N>& operator=(const MatrixMN<From, FromM, FromN>& src)
	{
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				at(i, j) = (i < FromM && j < FromN) ? src.at(i, j) : static_cast<T>(0);
			}
		}
		return *this;
	}

	MatrixMN<T, 1, N> row(int m) const
	{
		MatrixMN<T, 1, N> ret;
		for (int i = 0; i < N; i++)
			ret.at(0, i) = at(m, i);
		return ret;
	}

	void setRow(int m, const MatrixMN<T, 1, N>& r)
	{
		for (int i = 0; i < N; i++)
			at(m, i) = r.at(0, i);
	}

	MatrixMN<T, M, 1> column(int n) const
	{
		MatrixMN<T, M, 1> ret;
		for (int i = 0; i < M; i++)
			ret.at(i, 0) = at(i, n);
		return ret;
	}

	void setColumn(int n, const MatrixMN<T, M, 1>& c)
	{
		for (int i = 0; i < M; i++)
			at(i, n) = c.at(i, 0);
	}

	void swapRows(int m, int n)
	{
		T temp;
		if (m == n)
			return;
		for (int i = 0; i < N; i++) {
			temp = at(m, i);
			at(m, i) = at(n, i);
			at(n, i) = temp;
		}
	}

	void swapColumns(int m, int n)
	{
		T temp;
		if (m == n)
			return;
		for (int i = 0; i < M; i++) {
			temp = at(i, m);
			at(i, m) = at(i, n);
			at(i, n) = temp;
		}
	}

	MatrixMN<T, M, N> operator+(const MatrixMN<T, M, N>& rhs)
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] + rhs.data[i];
		return ret;
	}

	MatrixMN<T, M, N> operator+(T rhs)
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] + rhs;
		return ret;
	}

	MatrixMN<T, M, N> operator-(const MatrixMN<T, M, N>& rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] - rhs.data[i];
		return ret;
	}

	MatrixMN<T, M, N> operator-(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] - rhs;
		return ret;
	}

	MatrixMN<T, M, N> operator/(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] / rhs;
		return ret;
	}

	MatrixMN<T, M, N> operator*(T rhs) const
	{
		MatrixMN<T, M, N> ret;
		for (int i = 0; i < N * M; i++)
			ret.data[i] = data[i] * rhs;
		return ret;
	}

	template<typename rhsT, int rhsN>
	MatrixMN<T, M, rhsN> operator*(const MatrixMN<rhsT, N, rhsN>& rhs) const
	{
		MatrixMN<T, M, rhsN> ret;
		for (int h = 0; h < rhsN; h++) {
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					ret.at(i, h) += at(i, j) * rhs.at(j, h);
				}
			}
		}
		return ret;
	}

	MatrixMN<T, N, M> transpose() const
	{
		MatrixMN<T, N, M> ret;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				ret.at(i, j) = at(j, i);
		return ret;
	}

	std::string toString() const
	{
		std::stringstream oss;
		oss.setf(std::ios::fixed, std::ios::floatfield);
		oss.precision(2);
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				oss.width(7);
				oss << at(i, j) << " ";
			}
			oss << std::endl;
		}
		return oss.str();
	}

	T length() const
	{
		return std::sqrt(lengthSq());
	}

	T lengthSq() const
	{
		T ret = static_cast<T>(0);
		for (int i = 0; i < M; i++)
			ret += at(i, 0) * at(i, 0);
		return ret;
	}

	T sum() const
	{
		T ret = static_cast<T>(0);
		for (int i = 0; i < M; i++) {
			ret += at(i, 0);
		}
		return ret;
	}

	T minValue() const
	{
		T min = at(0,0);
		for (int i = 1; i < M; i++) {
			if (at(i,0) < min) {
				min = at(i,0);
			}
		}
		return min;
	}

	T maxValue() const
	{
		T max = at(0,0);
		for (int i = 1; i < M; i++) {
			if (at(i,0) > max) {
				max = at(i,0);
			}
		}
		return max;
	}

	MatrixMN<T, M, 1> normalize() const
	{
		MatrixMN<T, M, 1> ret;
		T s = length();
		if (s) {
			for (int i = 0; i < M; i++)
				ret.at(i, 0) = at(i, 0) / s;
		}
		return ret;
	}

	static const MatrixMN<T, 3, 1> cross(const MatrixMN<T, 3, 1>& u, const MatrixMN<T, 3, 1>& v)
	{
		return MatrixMN<T, 3, 1>(u.at(1,0) * v.at(2,0) - v.at(1,0) * u.at(2,0), u.at(2,0) * v.at(0,0) -
				v.at(2,0) * u.at(0,0), u.at(0,0) * v.at(1,0) - v.at(0,0) * u.at(1,0));
	}

	static T dot(const MatrixMN<T, M, 1>& u, const MatrixMN<T, M, 1>& v)
	{
		T ret = static_cast<T>(0);
		for (int i = 0; i < M; i++)
			ret += u.at(i, 0) * v.at(i, 0);
		return ret;
	}

	friend std::ostream& operator<<(std::ostream& os, const MatrixMN<T, M, N>& m)
	{
		os << m.toString();
		return os;
	}
};

template<typename T>
class Quaternion
{
public:
	T w, x, y, z;

public:
	Quaternion(const Quaternion<T>& q)
			: w(q.w), x(q.x), y(q.y), z(q.z)
	{
	}

	template<class FromT>
	Quaternion(const Quaternion<FromT>& q)
			: w(static_cast<T>(q.w)), x(static_cast<T>(q.x)), y(static_cast<T>(q.y)), z(static_cast<T>(q.z))
	{
	}

	Quaternion(T _w, const MatrixMN<T, 3, 1>& v)
			: w(_w), x(v.at(0,0)), y(v.at(1,0)), z(v.at(2,0))
	{
	}

	Quaternion(T _w = 0, T _x = 0, T _y = 0, T _z = 0)
			: w(_w), x(_x), y(_y), z(_z)
	{
	}

	template<typename FromT>
	Quaternion<T>& operator=(const Quaternion<FromT>& rhs)
	{
		w = static_cast<T>(rhs.w);
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		z = static_cast<T>(rhs.z);
		return *this;
	}

	Quaternion<T> operator+(const Quaternion<T>& rhs) const
	{
		return Quaternion<T>(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Quaternion<T> operator*(T rhs) const
	{
		return Quaternion<T>(w * rhs, x * rhs, y * rhs, z * rhs);
	}

	Quaternion<T> operator/(T rhs) const
	{
		return Quaternion<T>(w / rhs, x / rhs, y / rhs, z / rhs);
	}

	Quaternion<T> operator*(const Quaternion<T>& rhs) const
	{
		return Quaternion<T>(w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w);
	}

	Quaternion<T> operator-(const Quaternion<T>& rhs) const
	{
		return Quaternion<T>(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Quaternion<T>& operator+=(const Quaternion<T>& rhs)
	{
		w += rhs.w;
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Quaternion<T>& operator*=(T rhs)
	{
		w *= rhs;
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Quaternion<T>& operator-=(const Quaternion<T>& rhs)
	{
		w -= rhs.w;
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Quaternion<T>& operator*=(const Quaternion<T>& rhs)
	{
		Quaternion q = (*this) * rhs;
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}

	bool operator==(const Quaternion<T>& rhs) const
	{
		const Quaternion<T>& lhs = *this;
		return (std::fabs(w - rhs.w) < EPSILON) &&
				(std::fabs(x - rhs.x) < EPSILON) &&
				(std::fabs(y - rhs.y) < EPSILON) &&
				(std::fabs(z - rhs.z) < EPSILON);
	}

	bool operator!=(const Quaternion<T>& rhs) const
	{
		return !(*this == rhs);
	}

	Quaternion<T> operator-() const
	{
		return Quaternion<T>(-w, -x, -y, -z);
	}

	Quaternion<T> operator~() const
	{
		return Quaternion<T>(w, -x, -y, -z);
	}

	T length() const
	{
		return static_cast<T>(std::sqrt(w * w + x * x + y * y + z * z));
	}

	T lengthSq() const
	{
		return static_cast<T>(w * w + x * x + y * y + z * z);
	}

	Quaternion<T> normalize() const
	{
		Quaternion<T> q = *this;
		T len = length();
		q.w /= len;
		q.x /= len;
		q.y /= len;
		q.z /= len;
		return q;
	}

	MatrixMN<T, 3, 1> rotate(const MatrixMN<T, 3, 1>& v) const
	{
		Quaternion<T> r = (*this)*Quaternion<T>(0, v.at(0,0), v.at(1,0), v.at(2,0))*~(*this);
		return MatrixMN<T, 3, 1>(r.x, r.y, r.z);
	}

	MatrixMN<T, 4, 4> rotMatrix4() const
	{
		MatrixMN<T,4,4> ret;
		Quaternion<T> q = this->normalize();

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
		ret.at(0, 3) = 0;

		ret.at(1, 0) = 2 * (xy + zw);
		ret.at(1, 1) = 1 - 2 * (xx + zz);
		ret.at(1, 2) = 2 * (yz - xw);
		ret.at(1, 3) = 0;

		ret.at(2, 0) = 2 * (xz - yw);
		ret.at(2, 1) = 2 * (yz + xw);
		ret.at(2, 2) = 1 - 2 * (xx + yy);
		ret.at(2, 3) = 0;

		ret.at(3, 0) = 0;
		ret.at(3, 1) = 0;
		ret.at(3, 2) = 0;
		ret.at(3, 3) = 1;

		return ret;
	}

	MatrixMN<T, 3, 3> rotMatrix3() const
	{
		MatrixMN<T,4,4> ret;
		Quaternion<T> q = this->normalize();

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
		ret.at(0, 3) = 0;

		ret.at(1, 0) = 2 * (xy + zw);
		ret.at(1, 1) = 1 - 2 * (xx + zz);
		ret.at(1, 2) = 2 * (yz - xw);
		ret.at(1, 3) = 0;

		ret.at(2, 0) = 2 * (xz - yw);
		ret.at(2, 1) = 2 * (yz + xw);
		ret.at(2, 2) = 1 - 2 * (xx + yy);
		ret.at(2, 3) = 0;
		return ret;
	}

	Quaternion<T> ln()
	{
		Quaternion<T> Q(*this);
		MatrixMN<T,3,1> v = MatrixMN<T,3,1>(Q.x, Q.y, Q.z);
		T lV = v.length();
		double ac = std::acos(Q.w/Q.length());
		return Quaternion<T>(std::log(Q.length()), v.at(0,0)/lV*ac, v.at(1,0)/lV*ac, v.at(2,0)/lV*ac);
	}

	Quaternion<T> exp()
	{
		MatrixMN<T,3,1> v(x, y, z);
		T lV = v.length();
		double c = std::cos(lV);
		double s = std::sin(lV);
		double e = std::exp(w);
		return Quaternion<T>(e*c, e*v.at(0,0)/lV*s, e*v.at(1,0)/lV*s, e*v.at(2,0)/lV*s);
	}

	double angle()
	{
	    Quaternion<T> q = this->normalize();
	    return static_cast<double>(2*std::acos(q.w));
	}

	MatrixMN<T,3,1> axis() const
	{
		Quaternion<T> q = this->normalize();

		double ca2 = q.w;
		double sa2  = std::sqrt( 1.0 - ca2 * ca2 );
		if (fabs( sa2 ) < 0.0005)
		  sa2 = 1;
		return MatrixMN<T,3,1>(q.x/sa2, q.y/sa2, q.z/sa2);
	}

	static Quaternion<T> fromAngularVelocity(const MatrixMN<T,3,1>& omega /* Rad/Sec */, float deltaT)
	{
		Quaternion<T> deltaQ;
		MatrixMN<T,3,1> theta = omega * (0.5f * deltaT);
		float thetaMagSq = theta.lengthSq();
		float s;

		if(thetaMagSq * thetaMagSq / 24.0f < EPSILON) {
			deltaQ.w = 1.0f - thetaMagSq / 2.0f;
			s = 1.0f - thetaMagSq / 6.0f;
		} else {
			float thetaMag = std::sqrt(thetaMagSq);
			deltaQ.w = std::cos(thetaMag);
			s = sin(thetaMag) / thetaMag;
		}

		deltaQ.x = theta.at(0,0) * s;
		deltaQ.y = theta.at(1,0) * s;
		deltaQ.z = theta.at(2,0) * s;

		return deltaQ;
	}

	static Quaternion<T> fromAxisRot(MatrixMN<T,3,1> axis, float rad)
	{
		double sa2 = std::sin(rad / 2);
		double ca2 = std::cos(rad / 2);
		axis = axis * sa2;
		Quaternion<T> temp(ca2, axis.at(0,0), axis.at(1,0), axis.at(2,0));
		return temp;
	}

	static Quaternion<T> fromEulerAngles(T x, T y, T z)
	{
		Quaternion<T> ret = fromAxisRot(MatrixMN<T,3,1>(1, 0, 0), x) * fromAxisRot(MatrixMN<T,3,1>(0, 1, 0), y)
				* fromAxisRot(MatrixMN<T,3,1>(0, 0, 1), z);
		return ret;
	}

	static Quaternion<T> fromVectors(const MatrixMN<T, 3, 1>& u, const MatrixMN<T, 3, 1>& v)
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
			if (axis.lengthSq() < EPSILON * EPSILON) // pick another if colinear
				axis = MatrixMN<T, 3, 1>::cross(MatrixMN<T, 3, 1>(0, 1, 0), v0);
			axis.normalize();
			q = fromAxisRot(axis, 180);
		} else {
			double s = std::sqrt((1 + d) * 2);
			double invs = 1 / s;
			MatrixMN<T, 3, 1> c = MatrixMN<T, 3, 1>::cross(v0, v1);
			q.x = c.at(0,0) * invs;
			q.y = c.at(1,0) * invs;
			q.z = c.at(2,0) * invs;
			q.w = s * 0.5f;
			q = q.normalize();
		}
		return q;
	}


	static Quaternion<T> fromVectorsPartial(const MatrixMN<T,3,1>& u, const MatrixMN<T,3,1>& v, float part)
	{
		double d = MatrixMN<T, 3, 1>::dot(u, v);

		// If dot == 1, vectors are the same
		if (d >= (1.0f - EPSILON)) {
			return Quaternion<T>(1, 0, 0, 0);
		}
		Quaternion<T> Q = fromVectors(u,v);
		return fromAxisRot(Q.axis(), part * Q.angle());
	}

	static T dot(const Quaternion<T>& u, const Quaternion<T>& v)
	{
		return u.w*v.w + u.x*v.x + u.y*v.y + u.z*v.z;
	}


	/*
	 * Angle between two quaternions
	 */
	static double theta(const Quaternion<T>& u, const Quaternion<T>& v)
	{
		return std::acos(std::pow(dot(u, v),2.0)*2.0 - 1.0);
	}

	static Quaternion<T> nlerp(const Quaternion<T>& i, const Quaternion<T>& f, float blend)
	{
		Quaternion<T> result;

		if(dot(i, f) < 0.0f) {
			result = i * (1.0f - blend) - f * blend;
		} else {
			result = i * (1.0f - blend) + f * blend;
		}
		return result.normalize();
	}

	static MatrixMN<T,3,1> angularVelocity(Quaternion<T> i, Quaternion<T> f, float dT)
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

	static Quaternion<T> reciprocal(const Quaternion<T>& q)
	{
		Quaternion<T> rec;
		T norm = q.length();
		if (0 != norm) {
			rec = ~q / (norm*norm);
		} else {
			rec = q;
		}
		return rec;
	}

	std::string toString() const
	{
		std::stringstream oss;
		oss.setf(std::ios::fixed, std::ios::floatfield);
		oss.precision(2);
		oss << "( " << w << " + " << x << "i + "<< y << "j + "<< z << "k )";
		return oss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q)
	{
		os << q.toString();
		return os;
	}

};

typedef MatrixMN<float, 3, 3> Matrix3f;
typedef MatrixMN<double, 3, 3> Matrix3d;
typedef MatrixMN<int, 3, 3> Matrix3i;
typedef MatrixMN<float, 4, 4> Matrix4f;
typedef MatrixMN<double, 4, 4> Matrix4d;
typedef MatrixMN<int, 4, 4> Matrix4i;
typedef MatrixMN<float, 3, 1> Vector3f;
typedef MatrixMN<double, 3, 1> Vector3d;
typedef MatrixMN<int, 3, 1> Vector3i;
typedef MatrixMN<float, 4, 1> Vector4f;
typedef MatrixMN<double, 4, 1> Vector4d;
typedef MatrixMN<int, 4, 1> Vector4i;
typedef Quaternion<double> QuaternionD;

#ifdef MATRIX_NAMESPACE
}
#endif

#endif

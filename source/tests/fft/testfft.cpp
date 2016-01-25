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
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "d3math.h"

#include <iomanip>
#include <complex>
#include <cmath>

template<typename T>
void fft(const std::vector<std::complex<T>>& x, std::vector<std::complex<T>>& y)
{
	std::complex<T> j(0, 1.0);
	float pi = std::acos(-1);
	size_t N = x.size();

	for (size_t k = 0; k < N; k++) {
		std::complex<T> temp = 0;
		for (size_t n = 0; n < N; n++) {
			temp += x[n] * std::exp(j * -2.0f * (float)(k * n)/(float)N * pi);
		}
		y.push_back(temp);
	}
}

template<typename T>
void ifft(const std::vector<std::complex<T>>& y, std::vector<std::complex<T>>& x)
{
	std::complex<T> j(0, 1.0);
	float pi = std::acos(-1);
	size_t N = y.size();

	for (size_t k = 0; k < N; k++) {
		std::complex<T> temp = 0;
		for (size_t n = 0; n < N; n++) {
			temp += y[n] * std::exp(j * 2.0f * (float)(k * n)/(float)N * pi);
		}
		x.push_back(temp/(float)N);
	}
}

int main()
{
	std::vector<Vector3f> v{{1.0f,1.0f,1.0f}, {2.0,2.0}, {1.0}, {-1.0}, {1.5}, {-1.5} };
	std::vector<Vector3c> w;

	for (size_t k = 0; k < v.size(); k++)
		w.push_back(Vector3f::fft(v, k));

	for (auto i : w)
		std::cout << i << std::endl;
	std::cout << std::endl;
	for (size_t k = 0; k < w.size(); k++)
		v[k] = Vector3f::ifft(w, k);
	for (auto i : v)
		std::cout << i << std::endl;



}

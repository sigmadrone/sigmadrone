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
#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include "librexjson/rexjson++.h"
#include "d3math.h"

template<class T> Quaternion<T> quaternion_from_json_value(const rexjson::value& val)
{
	const rexjson::object& obj = val.get_obj();
	Quaternion<T> ret;
	ret.w = obj.at("w").get_real();
	ret.x = obj.at("x").get_real();
	ret.y = obj.at("y").get_real();
	ret.z = obj.at("z").get_real();
	return ret;
}

template<class T> rexjson::value quaternion_to_json_value(const Quaternion<T>& q)
{
	rexjson::object obj;
	obj["w"] = q.w;
	obj["x"] = q.x;
	obj["y"] = q.y;
	obj["z"] = q.z;
	return obj;
}

template <typename T, size_t M, size_t N>
MatrixMN<T, M, N> matrix_from_json_value(const rexjson::value& val)
{
	const rexjson::array& arr = val.get_array();
	MatrixMN<T, M, N> ret;
	for (size_t i = 0; i < M; i++)
		for (size_t j = 0; j < N; j++)
			ret.at(i, j) = arr.at(j * M + i).get_real();
	return ret;
}

template <typename T, size_t M, size_t N>
rexjson::value matrix_to_json_value(const MatrixMN<T, M, N>& m)
{
	rexjson::array arr;
	for (size_t i = 0; i < M; i++)
		for (size_t j = 0; j < N; j++)
			arr.push_back(m.at(i, j));
	return arr;
}

template <typename T>
rexjson::value vector_to_json_value(const std::vector<T>& v)
{
	rexjson::array arr;
	for (size_t i = 0; i < v.size(); i++)
			arr.push_back(v.at(i));
	return arr;
}


#endif

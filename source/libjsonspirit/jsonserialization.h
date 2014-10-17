#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include "jsontypes.h"
#include "matrix.h"

template<class T> Quaternion<T> quaternion_from_json_value(const json::value& val)
{
	const json::object& obj = val.get_obj();
	Quaternion<T> ret;
	ret.w = obj.at("w").get_value<T>();
	ret.x = obj.at("x").get_value<T>();
	ret.y = obj.at("y").get_value<T>();
	ret.z = obj.at("z").get_value<T>();
	return ret;
}

template<class T> json::value quaternion_to_json_value(const Quaternion<T>& q)
{
	json::object obj;
	obj["w"] = q.w;
	obj["x"] = q.x;
	obj["y"] = q.y;
	obj["z"] = q.z;
	return obj;
}

template <typename T, int M, int N>
MatrixMN<T, M, N> matrix_from_json_value(const json::value& val)
{
	const json::array& arr = val.get_array();
	MatrixMN<T, M, N> ret;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			ret.at(i, j) = arr.at(j * M + i).get_value<T>();
	return ret;
}

template <typename T, int M, int N>
json::value matrix_to_json_value(const MatrixMN<T, M, N>& m)
{
	json::array arr;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			arr.push_back(m.at(i, j));
	return arr;
}


#endif

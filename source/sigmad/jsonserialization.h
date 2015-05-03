#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include "librexjson/rexjson++.h"
#include "matrix.h"

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

template <typename T, int M, int N>
MatrixMN<T, M, N> matrix_from_json_value(const rexjson::value& val)
{
	const rexjson::array& arr = val.get_array();
	MatrixMN<T, M, N> ret;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			ret.at(i, j) = arr.at(j * M + i).get_real();
	return ret;
}

template <typename T, int M, int N>
rexjson::value matrix_to_json_value(const MatrixMN<T, M, N>& m)
{
	rexjson::array arr;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			arr.push_back(m.at(i, j));
	return arr;
}


#endif

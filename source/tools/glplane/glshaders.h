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
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;


class GlShaders
{
public:
	GlShaders();
	~GlShaders();

	void AddFile(unsigned int shaderType, const string &fileName) throw(runtime_error);
	void AddText(unsigned int shaderType, const string &source) throw(runtime_error);
	void Link() throw(runtime_error);

private:
	void CreateProgram();
	std::string GetInfoLog(unsigned int obj);

public:
	unsigned int m_program;
	vector<unsigned int> m_shaders;
};

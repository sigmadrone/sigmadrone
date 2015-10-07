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
#include <string.h>
#include "glshaders.h"

#include "GL/glew.h"


GlShaders::GlShaders()
	: m_program(0)
{
}

GlShaders::~GlShaders()
{
	for (unsigned int i = 0; i < m_shaders.size(); i++) {
		glDetachShader(m_program, m_shaders.at(i));
		glDeleteShader(m_shaders.at(i));
	}
	glDeleteProgram(m_program);
}

std::string GlShaders::GetInfoLog(unsigned int obj)
{
	int infologLength = 0;
	int maxLength = 0;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
 	char *infoLog = new char[maxLength + 1];
	memset(infoLog, 0, maxLength + 1);
	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
	std::string ret(infoLog);
	delete infoLog;
	return ret;
}

void GlShaders::AddText(unsigned int shaderType, const string &source) throw(runtime_error)
{
	const char* text = source.c_str();
	GLint status = 0;
	GLuint shader = 0;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &text, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if ((status != GL_TRUE )) {
		std::string errstr("Shader compilation failed: " + GetInfoLog(shader) + "\n" + source);
		glDeleteShader(shader);
		throw(std::runtime_error(errstr));
	}
	glAttachShader(m_program, shader);
	m_shaders.push_back(shader);
}

void GlShaders::AddFile(unsigned int shaderType, const string &fileName) throw(runtime_error)
{
	fstream shaderFile(fileName.c_str(), ios::in);
	std::stringstream buffer;

	/* The program will be created only once */
	CreateProgram();

	if (!shaderFile.is_open()) {
		/*
		 * Error
		 */
		std::string errstr("ERROR: File not found: " + fileName);
		cerr << errstr << endl;
		throw(std::runtime_error(errstr));
	}
	buffer << shaderFile.rdbuf();
	shaderFile.close();
	std::string source(buffer.str());
	AddText(shaderType, source);
}

void GlShaders::Link() throw(runtime_error)
{
	GLint status;
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if ((status != GL_TRUE )) {
		/*
		 * Error
		 */
		std::string errstr("Shader link failed: " + GetInfoLog(m_program));
		throw(std::runtime_error(errstr));
	}
}

void GlShaders::CreateProgram()
{
	if (!m_program)
		m_program = glCreateProgram();
}

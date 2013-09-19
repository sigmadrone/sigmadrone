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

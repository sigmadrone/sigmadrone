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

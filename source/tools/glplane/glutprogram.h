#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"

class GlutProgram
{
public:
	GlutProgram(unsigned int mode = GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA,
			int x = 400, int y = 200, int sizeX = 800, int sizeY = 600);
	virtual void Init(int argc, char *argv[]);

protected:
	int m_mode;
	int m_x;
	int m_y;
	int m_sizeX;
	int m_sizeY;
};

#include <iostream>
using namespace std;

#include "glutprogram.h"

GlutProgram::GlutProgram(unsigned int mode,	int x, int y, int sizeX, int sizeY) :
			m_mode(mode), m_x(x), m_y(y), m_sizeX(sizeX), m_sizeY(sizeY)
{

}


void GlutProgram::Init(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(m_mode);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(m_sizeX, m_sizeY);
	glutInitWindowPosition(m_x, m_y);
	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CLIP_PLANE0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

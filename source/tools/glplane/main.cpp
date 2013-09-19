#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

#include <string.h>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glshaders.h"
#include "glutprogram.h"
#include "glshape.h"
#include "plane.h"
#include "matrix.h"

#define ALPHA1 0.95f
#define ALPHA2 1.0f
#define SCALE 1.75
#define WINDOW_SIZE_FACTOR 4
#define BUFFER_OFFSET(i) ((void*)(i))

Matrix4f P(Matrix4f::createFrustumMatrix(-SCALE, SCALE, -SCALE, SCALE, 10, 35));

int gDebug = 0;
GlShape gPlane;
GLuint gProgram;
void IdleFunction(void);

Vector4f applyW(const Vector4f &v)
{
	Vector4f ret = v;
	float w = v.at(3, 0);

	if (w) {
		ret.at(0, 0) /= w;
		ret.at(1, 0) /= w;
		ret.at(2, 0) /= w;
		ret.at(3, 0) /= w;
	}
	return ret;
}


void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gPlane.Render();
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{

	GlutProgram prog(GLUT_MULTISAMPLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA, 800, 900, 160 * WINDOW_SIZE_FACTOR, 160 * WINDOW_SIZE_FACTOR);
	GlShaders shaders;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--debug") == 0) {
			gDebug = 1;
		}
	}
	prog.Init(argc, argv);
	shaders.AddFile(GL_VERTEX_SHADER, "shapes.vert");
	shaders.AddFile(GL_FRAGMENT_SHADER, "shapes.frag");
	shaders.Link();
	glUseProgram(shaders.m_program);
	InitPlaneShape(gPlane);
	gProgram = shaders.m_program;
	glutDisplayFunc(display);
	glutIdleFunc(IdleFunction);

	Matrix4f m(P * Matrix4f::createTranslationMatrix(0, 0, -12) * Matrix4f::createRotationMatrix(DEG2RAD(-88), DEG2RAD(0), DEG2RAD(180)));

	GLuint mLoc = glGetUniformLocation(gProgram, "M");
	if (mLoc >= 0)
		glUniformMatrix4fv(mLoc, 1, GL_FALSE, m.data);

	glutMainLoop();

	cout << "It works!" << endl;
	cout << "Program ID: " << shaders.m_program << endl;
	glUseProgram(0);
	return 0;
}

void IdleFunction(void)
{
	fd_set rset;
	struct timeval tv;
	int maxfd;
	Matrix4f M;
	FD_ZERO (&rset);
	FD_SET (fileno (stdin), &rset);

	maxfd = fileno (stdin);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

again:
	if (select (maxfd + 1, &rset, NULL, NULL, &tv) > 0) {
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		if (fgets(buffer, sizeof(buffer) - 1, stdin)) {
			if (strncmp(buffer, "-->", 3) == 0) {
				if (gDebug)
					fputs(buffer, stdout);
				goto again;
			}
			sscanf(buffer, "%f %f %f %f %f %f %f %f %f",
					&M.at(0,0), &M.at(0,1), &M.at(0,2),
					&M.at(1,0), &M.at(1,1), &M.at(1,2),
					&M.at(2,0), &M.at(2,1), &M.at(2,2));
			M.at(3,3) = 1.0f;
			if (gDebug) {
				fprintf(stdout, "%5.9f %5.9f %5.9f %5.9f %5.9f %5.9f %5.9f %5.9f %5.9f\n",
								M.at(0,0), M.at(0,1), M.at(0,2),
								M.at(1,0), M.at(1,1), M.at(1,2),
								M.at(2,0), M.at(2,1), M.at(2,2));
			}
			GLuint mLoc;
			Matrix4f m(P * Matrix4f::createTranslationMatrix(0, 0, -12) * Matrix4f::createRotationMatrix(DEG2RAD(-88), DEG2RAD(0), DEG2RAD(180)) * M);

			mLoc = glGetUniformLocation(gProgram, "M");
			if (mLoc >= 0)
				glUniformMatrix4fv(mLoc, 1, GL_FALSE, m.data);
			glutPostRedisplay();

		}
	}
}


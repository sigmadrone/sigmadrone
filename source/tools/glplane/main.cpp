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
#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>
#include <string.h>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glshaders.h"
#include "glutprogram.h"
#include "glshape.h"
#include "plane.h"
#include "d3math.h"
#include "libcmdargs/cmdargs.h"
#include "libhttp/http_client.hpp"
#include "librexjsonrpc/rpcclienthttp.h"
#include "jsonserialization.h"


#define ALPHA1 0.95f
#define ALPHA2 1.0f
#define SCALE 1.75f
#define WINDOW_SIZE_FACTOR 4
#define BUFFER_OFFSET(i) ((void*)(i))



Matrix4f P(create_frustum_matrix<float>(-SCALE, SCALE, -SCALE, SCALE, 10.0f, 35.0f));

static cmd_arg_spec g_argspec[] = {
		{"help",			"h",	"Display this help", CMD_ARG_BOOL},
		{"rpcserver",		"",		"RPC server name", CMD_ARG_STRING},
		{"rpcport",			"",		"RPC server port. Default: 18222", CMD_ARG_STRING},

};


int gDebug = 0;
GlShape gPlane;
GLuint gProgram;
void IdleFunction(void);
void RpcIdleFunction(void);
QuaternionF remapQ = QuaternionF::fromAxisRot(Vector3f(1,0,0), M_PI) * QuaternionF::fromAxisRot(Vector3f(0,0,1), M_PI/2);

Vector4f applyW(const Vector4f &v)
{
	if (v[3])
		return v/v[3];
	return v;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gPlane.Render();
	glutSwapBuffers();
}

static std::string rpcserver;
static std::string rpcport;
static rpc_client_http* rpcclient = NULL;

int main(int argc, char *argv[])
{
	cmd_args args;

	try {
		args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
		args.parse_command_line(argc, (const char**)argv);
		if (!args.get_value("help").empty()) {
			std::cout << argv[0] << " <options>" << std::endl;
			std::cout << args.get_help_message() << std::endl;
			return 0;
		}
		rpcserver = args.get_value("rpcserver");
		rpcport = args.get_value("rpcport", "18222");
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	if (!rpcserver.empty())
		rpcclient = new rpc_client_http(rpcserver, rpcport, 30000);

	GlutProgram prog(GLUT_MULTISAMPLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA, 800, 900, 160 * WINDOW_SIZE_FACTOR, 160 * WINDOW_SIZE_FACTOR);
	GlShaders shaders;
	int junk = pthread_getconcurrency();
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--debug") == 0) {
			gDebug = 1;
		}
		junk++;
	}
	prog.Init(argc, argv);
	shaders.AddFile(GL_VERTEX_SHADER, "shapes.vert");
	shaders.AddFile(GL_FRAGMENT_SHADER, "shapes.frag");
	shaders.Link();
	glUseProgram(shaders.m_program);
	InitPlaneShape(gPlane);
	gProgram = shaders.m_program;
	glutDisplayFunc(display);
	glutIdleFunc(rpcclient ? RpcIdleFunction : IdleFunction);

	Matrix4f m(P * create_translation_matrix<float>(0, 0, -12) * create_rotation_matrix<float>(DEG2RAD(-88), DEG2RAD(0), DEG2RAD(180)));

	GLint mLoc = glGetUniformLocation(gProgram, "M");
	if (mLoc >= 0)
		glUniformMatrix4fv(mLoc, 1, GL_TRUE, m);

	glutMainLoop();

	cout << "It works!" << endl;
	cout << "Program ID: " << shaders.m_program << endl;
	glUseProgram(0);
	if (rpcclient)
		delete rpcclient;
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
	QuaternionF q;

again:
	if (select (maxfd + 1, &rset, NULL, NULL, &tv) > 0) {
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		if (fgets(buffer, sizeof(buffer) - 1, stdin)) {
			if (strncmp(buffer, "\n", 1) == 0 || strncmp(buffer, "\r\n", 2) == 0)
				goto again;
			if (strncmp(buffer, "-->", 3) == 0) {
				if (gDebug)
					fputs(buffer, stdout);
				goto again;
			}
			sscanf(buffer, "%f %f %f %f", &q.w, &q.x, &q.y, &q.z);
			q = q * remapQ;
			M = q.rotMatrix4();
			if (gDebug) {
				fprintf(stdout, "%5.9lf %5.9lf %5.9lf %5.9lf\n", q.w, q.x, q.y, q.z);
			}
			GLint mLoc;
			Matrix4f m(P * create_translation_matrix<float>(0, 0, -12) * create_rotation_matrix<float>(DEG2RAD(-88), DEG2RAD(180), DEG2RAD(90)) * M);

			mLoc = glGetUniformLocation(gProgram, "M");
			if (mLoc >= 0)
				glUniformMatrix4fv(mLoc, 1, GL_TRUE, m);
			glutPostRedisplay();

		}
	}
}


void RpcIdleFunction(void)
{
	QuaternionF q = QuaternionF::identity;
	Matrix4f M;

	try {
		rexjson::value val = rpcclient->call("/", "sd_get_attitude");
		q = quaternion_from_json_value<double>(val);
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	q = q * remapQ;
	M = q.rotMatrix4();
	GLint mLoc;
	Matrix4f m(P * create_translation_matrix<float>(0, 0, -12) * create_rotation_matrix<float>(DEG2RAD(-88), DEG2RAD(180), DEG2RAD(90)) * M);
	mLoc = glGetUniformLocation(gProgram, "M");
	if (mLoc >= 0)
		glUniformMatrix4fv(mLoc, 1, GL_TRUE, m);
	glutPostRedisplay();
}

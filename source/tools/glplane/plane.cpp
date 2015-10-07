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
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "matrix.h"
#include "plane.h"

#define ALPHA1 1.00f


void InitPlaneShape(GlShape &plane)
{
	plane.AddVertex(-0.1f, -1.0f, 0.0f,			1.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1, 0);			// front body 0
	plane.AddVertex(0.1f, -1.0f, 0.0f,			1.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1, 0);			// front body 1
	plane.AddVertex(-0.1f,  1.0f, 0.0f,			1.0f, 0.0f, 1.0f, ALPHA1, 	-1,  1, 0);			// back body 2
	plane.AddVertex(0.1f,  1.0f, 0.0f,			1.0f, 0.0f, 1.0f, ALPHA1, 	 1,  1, 0);			// back body 3

	plane.AddVertex(0.1f,  -0.75f, 0.0f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1, 0);	// head wing front 4
	plane.AddVertex(1.5f,   0.00f, 0.2f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 1,  0, 0);	// head wing front 5
	plane.AddVertex(1.5f,   0.30f, 0.2f,		1.0f, 0.0f, 1.0f, ALPHA1, 	 1,  0, 0);	// head wing back 6
	plane.AddVertex(0.1f,   0.00f, 0.0f,		0.0f, 1.0f, 1.0f, ALPHA1, 	 1,  1, 0);	// head wing back 7

	plane.AddVertex(-0.1f, -0.75f, 0.0f, 		1.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1, 0);	// head wing front left 8
	plane.AddVertex(-1.5f,   0.00f, 0.2f, 		1.0f, 1.0f, 1.0f, ALPHA1, 	-1,  0, 0);	// head wing front left 9
	plane.AddVertex(-1.5f,   0.30f, 0.2f, 		1.0f, 0.0f, 1.0f, ALPHA1, 	-1,  1, 0);	// head wing front left 10
	plane.AddVertex(-0.1f,   0.00f, 0.0f, 		0.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1, 0);	// head wing front left 11

	plane.AddVertex(0.1f,   0.4f,  0.0f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1, 0);	// back wing front 12
	plane.AddVertex(0.4f,   1.00f, 0.3f,		1.0f, 0.0f, 1.0f, ALPHA1, 	1, -1, 1);	// back wing front 13
	plane.AddVertex(0.4f,   1.20f, 0.3f,		0.0f, 1.0f, 1.0f, ALPHA1, 	1,  1, 1);	// back wing front 14

	plane.AddVertex(-0.1f,   0.4f,  0.0f,		1.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1, 0);	// back wing front 15
	plane.AddVertex(-0.4f,   1.00f, 0.3f,		1.0f, 0.0f, 1.0f, ALPHA1, 	-1, -1, 1);	// back wing front 16
	plane.AddVertex(-0.4f,   1.20f, 0.3f,		0.0f, 1.0f, 1.0f, ALPHA1, 	-1,  1, 1);	// back wing front 17

	plane.AddVertex(0.0f,  -1.75f,-0.05f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 0, -1, -0.1); // cockpit front   18
	plane.AddVertex(0.0f,  -1.0f,  0.1f,		0.0f, 1.0f, 1.0f, ALPHA1, 	 0,  0,  1);	 // cockpit middle  19
	plane.AddVertex(0.0f,   1.0f,  0.0f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 0,  1,  1);	 // cockpit middle  20

	plane.AddVertex(-0.01f, -0.5f, -0.001f,		0.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1,  0);	// left engine front 21
	plane.AddVertex(-0.1f,  -0.5f, -0.001f,		0.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1,  0);  // left engine front 22
	plane.AddVertex(-0.01f,  1.1f, -0.001f,		0.0f, 1.0f, 1.0f, ALPHA1, 	 1,  1,  0);  // left engine front 23
	plane.AddVertex(-0.1f,   1.1f, -0.001f,		0.0f, 1.0f, 1.0f, ALPHA1, 	-1,  1,  0);  // left engine front 24
	plane.AddVertex(-0.01f, -0.2f, -0.05f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1,  -1);	 // left engine front 25
	plane.AddVertex(-0.1f,  -0.2f, -0.05f,		0.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1,  -1);  // left engine front 26
	plane.AddVertex(-0.01f,  1.1f, -0.05f,		1.0f, 1.0f, 1.0f, ALPHA1, 	 1,  1,  -1);  // left engine front 27
	plane.AddVertex(-0.1f,   1.1f, -0.05f,		0.0f, 1.0f, 1.0f, ALPHA1, 	-1,  1,  -1);  // left engine front 28

	plane.AddVertex(+0.1f,  -0.5f,  -0.001f,	0.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1, 0); // right engine front 29
	plane.AddVertex(+0.01f, -0.5f,  -0.001f,	0.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1, 0); // right engine front 30
	plane.AddVertex(+0.1f,   1.1f,  -0.001f,	0.0f, 1.0f, 1.0f, ALPHA1, 	 1,  1, 0); // right engine front 31
	plane.AddVertex(+0.01f,  1.1f,  -0.001f,	0.0f, 1.0f, 1.0f, ALPHA1, 	-1,  1, 0); // right engine front 32

	plane.AddVertex(+0.1f,  -0.2f,  -0.05f,		0.0f, 1.0f, 1.0f, ALPHA1, 	 1, -1, 1); // right engine front 33
	plane.AddVertex(+0.01f, -0.2f,  -0.05f,		1.0f, 1.0f, 1.0f, ALPHA1, 	-1, -1, 1); // right engine front 34
	plane.AddVertex(+0.1f,   1.1f,  -0.05f,		0.0f, 1.0f, 1.0f, ALPHA1, 	 1,  1, 1); // right engine front 35
	plane.AddVertex(+0.01f,  1.1f,  -0.05f,		1.0f, 1.0f, 1.0f, ALPHA1, 	-1,  1, 0); // right engine front 36

	plane.AddTriangle(0, 1, 2);
	plane.AddTriangle(1, 3, 2);
	plane.AddTriangle(4, 5, 6);

	plane.AddTriangle(6, 7, 4);
	plane.AddTriangle(8, 10,9);
	plane.AddTriangle(8, 11,10);
	plane.AddTriangle(12,13,14);
	plane.AddTriangle(14,3,12);
	plane.AddTriangle(15,2,17);
	plane.AddTriangle(17,16,15);
	plane.AddTriangle(1, 0, 18);
	plane.AddTriangle(1, 19,18);
	plane.AddTriangle(19, 0,18);
	plane.AddTriangle(1, 20,19);
	plane.AddTriangle(20, 0,19);

	/* Engine Left */
	plane.AddTriangle(21, 23, 22);
	plane.AddTriangle(22, 23, 24);

	plane.AddTriangle(25, 27, 26);
	plane.AddTriangle(26, 27, 28);

	plane.AddTriangle(25, 21, 23);
	plane.AddTriangle(23, 27, 25);

	plane.AddTriangle(26, 22, 24);
	plane.AddTriangle(24, 28, 26);		/* 23 */

	/* Engine Right */
	plane.AddTriangle(29, 31, 32);
	plane.AddTriangle(32, 30, 29);

	plane.AddTriangle(33, 35, 36);
	plane.AddTriangle(36, 34, 33);

	plane.AddTriangle(33, 29, 31);
	plane.AddTriangle(31, 35, 33);

	plane.AddTriangle(34, 30, 32);
	plane.AddTriangle(32, 36, 34);		/* 31 */

	plane.GpuMap();
}

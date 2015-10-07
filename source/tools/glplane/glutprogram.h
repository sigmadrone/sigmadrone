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

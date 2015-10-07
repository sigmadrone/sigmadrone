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
#include "string.h"

#include "glshape.h"
#include "GL/glew.h"

#define BUFFER_OFFSET(i) ((void*)(i))

GlShape::GlShape() :
	m_vao(0), m_vbo(0), m_ebo(0), m_tex(0)
{
	memset(&m_texinfo, 0, sizeof(m_texinfo));
}

void GlShape::GpuAlloc()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), BUFFER_OFFSET(sizeof(POSITION)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), BUFFER_OFFSET(sizeof(POSITION) + sizeof(COLOR)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), BUFFER_OFFSET(sizeof(POSITION) + sizeof(COLOR) + sizeof(NORMAL)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index.size() * sizeof(triangle_type), m_index.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenTextures(1, &m_tex);
}

GlShape::~GlShape()
{
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
	if (m_ebo)
		glDeleteBuffers(1, &m_ebo);
	if (m_tex)
		glDeleteTextures(1, &m_tex);
	if (m_vao) {
		glBindVertexArray(m_vao);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_texinfo.data)
		delete m_texinfo.data;
}

void GlShape::GpuMap()
{
	if (!m_vao)
		GpuAlloc();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(vertex_type), m_vertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index.size() * sizeof(triangle_type), m_index.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (m_texinfo.data) {
		glBindTexture(GL_TEXTURE_2D, m_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texinfo.width, m_texinfo.height, 0, m_texinfo.format, GL_UNSIGNED_BYTE, m_texinfo.data);
	    // Nice trilinear filtering.
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	    glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GlShape::Render()
{
	if (m_vao && m_ebo) {
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBindTexture(GL_TEXTURE_2D, m_tex);
		glDrawElements(GL_TRIANGLES, m_index.size() * 3, GL_UNSIGNED_SHORT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}


void GlShape::AddVertex(float x, float y, float z, float r, float g, float b, float a, float Xn, float Yn, float Zn, float U, float V)
{
	VERTEX v;
	v.p.x = x;
	v.p.y = y;
	v.p.z = z;
	v.p.w = 1.0f;
	v.c.r = r;
	v.c.g = g;
	v.c.b = b;
	v.c.a = a;
	v.n.x = Xn;
	v.n.y = Yn;
	v.n.z = Zn;
	v.t.u = U;
	v.t.v = V;
	m_vertex.push_back(v);
}

void GlShape::AddTriangle(unsigned int v1, unsigned int v2, unsigned int v3)
{
	TRIANGLE t;
	t.v1 = v1;
	t.v2 = v2;
	t.v3 = v3;
	m_index.push_back(t);
}

void GlShape::SetTexture(unsigned char *data, unsigned int width, unsigned int height, unsigned int format)
{
	if (m_texinfo.data)
		delete m_texinfo.data;
	m_texinfo.data = data;
	m_texinfo.width = width;
	m_texinfo.height = height;
	m_texinfo.format = format;
}

bool GlShape::LoadBitmap(const char * imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file) {
		printf("Image could not be opened\n");
		return false;
	}
	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
	    printf("Not a correct BMP file\n");
	    return false;
	}
	if (header[0] != 'B' || header[1] != 'M'){
	    printf("Not a correct BMP file\n");
	    return false;
	}
	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that way
	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	SetTexture(data, width, height, GL_BGR);
	return true;
}

#include "iso-tasty/renderer.h"

#ifndef NO_GL
	#ifdef _WIN32
	#include <windows.h>
	#endif

	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <math.h>
#include <stdio.h>

IsoTasty::Renderer::Renderer() {
}

void IsoTasty::Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glColor3d(1.0, 1.0, 1.0);
}

bool IsoTasty::Renderer::initializeViewport(unsigned int width, unsigned int height) {
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
 
	glViewport(0, 0, width, height);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	return true;
}

void IsoTasty::Renderer::setProjection(unsigned int width, unsigned int height, double rotation, double translationX, double translationZ, double zoom) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double dist = sqrt(1 / 3.0);
	double aspect = (double)width/(double)height;
	glOrtho(-aspect*width/2.0, aspect*width/2.0, -aspect*height/2.0, aspect*height/2.0, -aspect*height*4, aspect*height*4);
	glRotated(35.264, 1.0, 0.0, 0.0);
	glRotated(rotation, 0.0, 1.0, 0.0);
	glScaled(32.0f * zoom, 32.0f * zoom, 32.0f * zoom);
	glTranslated(translationX, 0, translationZ);
	 
	glMatrixMode(GL_MODELVIEW);
}

void IsoTasty::Renderer::drawArrays(const float vertices[], const float normals[], const float colors[], const unsigned char indices[], unsigned int num) {
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, 0, normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawElements(GL_QUADS, num, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void IsoTasty::Renderer::drawCube(float x, float y, float z, float width, float height, float depth) {
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	// vertex coords array
	static const float vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
						  1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
						  1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
						  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
						  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
						  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

	// normal array
	static const float normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
						 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
						 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
						 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
						 0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
						 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

	// color array
	static const float colors[] = {0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v1-v2-v3
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v3-v4-v5
						1,1,1,  0,1,1,  0,1,0,  1,1,0,                                   // v0-v5-v6-v1
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v1-v6-v7-v2
						0,0,0,  0,0,1,  1,0,1,  1,0,0,                                   // v7-v4-v3-v2
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0};// v4-v7-v6-v5

	static const unsigned char indices[] = {0,1,2,3,
						 4,5,6,7,
						 8,9,10,11,
						 12,13,14,15,
						 16,17,18,19,
						 20,21,22,23};

	glCullFace(GL_BACK);

	float half_width = width / 2.0f;
	float half_height = height / 2.0f;
	float half_depth = depth / 2.0f;

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(half_width, half_height, half_depth);

	drawArrays(vertices, normals, colors, indices, 24);

	glPopMatrix();
}

void IsoTasty::Renderer::test() {
	float width = 1.0f;
	float height = 1.0f;
	float depth = 1.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	// vertex coords array
	float* vertices;
	float* colors;
	float* normals;
	unsigned char* indices;

	vertices = new float[(4+20)*3*2];
	colors = new float[(4+20)*3*2];
	normals = new float[(4+20)*3*2];
	indices = new unsigned char[12*9*4];

	glCullFace(GL_BACK);
	
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(20.f, 20.0f, 20.0f);

	// Look for change in slope from previous tile
	// If there is a change in slope... interpolate
	// If there is no change in slope... linear

	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	for (int j=0; j<60; j+=6) {
		int k = j+1;
		int l = j+2;
		float var = (float)(rand() % 100) / 100.0f;
		var *= 0.05f;
		vertices[j] = 1.f - ((float)j/54.0f)*2.0f;
		vertices[k] = 1.f + (((float)j)/54.0f)*0.5f; // linear
		//vertices[l] = 1.f + (((float)j)/54.0f)*0.3f; // curve
		vertices[l] = 1.f;
		vertices[j+3] = vertices[j];
		vertices[k+3] = -1.f;
		vertices[l+3] = vertices[l];
		normals[j] = 0.0f;
		normals[k] = 1.0f;
		normals[l] = 0.0f;
		normals[j+3] = 0.0f;
		normals[k+3] = -1.0f;
		normals[l+3] = 0.0f;
		colors[j] = 1.f - ((float)j/54.0f);
		colors[k] = 1.0f;
		colors[l] = 1.f - ((float)j/54.0f);
		colors[j+3] = 1.f - ((float)j/54.0f);
		colors[k+3] = 1.0f;
		colors[l+3] = 1.f - ((float)j/54.0f);
	}

	for (int j=60; j<120; j+=6) {
		int k = j+1;
		int l = j+2;
		float var = (float)(rand() % 100) / 100.0f;
		var *= 0.05f;
		vertices[j] = 1.f - (((float)j-60.0f)/54.0f)*2.0f;
		vertices[k] = 1.f + (((float)j-60.0f)/54.0f)*0.5f;
		vertices[l] = -1.f;
		vertices[j+3] = vertices[j];
		vertices[k+3] = -1.f;
		vertices[l+3] = vertices[l];
		normals[j] = 0.0f;
		normals[k] = -1.0f;
		normals[l] = 0.0f;
		normals[j+3] = 0.0f;
		normals[k+3] = 1.0f;
		normals[l+3] = 0.0f;
		colors[j] = (((float)j-60.0f)/54.0f);
		colors[k] = 1.0f;
		colors[l] = (((float)j-60.0f)/54.0f);
		colors[j+3] = (((float)j-60.0f)/54.0f);
		colors[k+3] = 1.0f;
		colors[l+3] = (((float)j-60.0f)/54.0f);
	}	
	
	for (int i = 0; i < 9; i++) {
		// top rects
		indices[(i*12)+0] = 0+(i*2);
		indices[(i*12)+1] = 20+(i*2);
		indices[(i*12)+2] = 22+(i*2);
		indices[(i*12)+3] = 2+(i*2);

		// left side
		indices[(i*12)+4] = 0+(i*2);
		indices[(i*12)+5] = 2+(i*2);
		indices[(i*12)+6] = 3+(i*2);
		indices[(i*12)+7] = 1+(i*2);

		// right side
		indices[(i*12)+8] = 20+(i*2);
		indices[(i*12)+9] = 21+(i*2);
		indices[(i*12)+10] = 23+(i*2);
		indices[(i*12)+11] = 22+(i*2);
	}

	// side
	
	indices[9*12+0] = 0;
	indices[9*12+1] = 1;
	indices[9*12+2] = 21;
	indices[9*12+3] = 20;
	indices[9*12+4] = 18;
	indices[9*12+5] = 38;
	indices[9*12+6] = 39;
	indices[9*12+7] = 19;

	drawArrays(vertices, normals, colors, indices, 3*4*9+8);

	delete [] vertices;
	delete [] colors;
	delete [] normals;
	delete [] indices;

	glPopMatrix();
}

void IsoTasty::Renderer::drawSphere(float x, float y, float z, float width, float height, float depth) {
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	// vertex coords array
	static const float vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
						  1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
						  1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
						  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
						  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
						  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

	// normal array
	static const float normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
						 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
						 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
						 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
						 0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
						 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

	// color array
	static const float colors[] = {0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v1-v2-v3
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v3-v4-v5
						1,1,1,  0,1,1,  0,1,0,  1,1,0,                                   // v0-v5-v6-v1
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v1-v6-v7-v2
						0,0,0,  0,0,1,  1,0,1,  1,0,0,                                   // v7-v4-v3-v2
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0};// v4-v7-v6-v5

	static const unsigned char indices[] = {0,1,2,3,
						 4,5,6,7,
						 8,9,10,11,
						 12,13,14,15,
						 16,17,18,19,
						 20,21,22,23};

	glCullFace(GL_BACK);

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(width, height, depth);

	drawArrays(vertices, normals, colors, indices, 24);

	glPopMatrix();
}

void IsoTasty::Renderer::drawSquare(float x, float y, float z, float width, float depth) {
	float half_width = width / 2.0f;
	float half_depth = depth / 2.0f;
	
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(half_width, 1.0f, half_depth);

	glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
	
	// Top
	glBegin(GL_LINE_LOOP);
	glVertex3f( 1.0f, 0.0f,-1.0f);
	glVertex3f(-1.0f, 0.0f,-1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f( 1.0f, 0.0f, 1.0f);
	glEnd();
	
	glPopMatrix();
}

void IsoTasty::Renderer::drawTileTop(float x, float y, float z, float width, float height, float depth, float heights[4]) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(width, height, depth);

	glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
	
	// Top
	glBegin(GL_LINE_LOOP);
	glVertex3f( 1.0f, heights[IsoTasty::TOP_RIGHT],-1.0f);
	glVertex3f(-1.0f, heights[IsoTasty::TOP_LEFT],-1.0f);
	glVertex3f(-1.0f, heights[IsoTasty::BOT_LEFT], 1.0f);
	glVertex3f( 1.0f, heights[IsoTasty::BOT_RIGHT], 1.0f);
	glEnd();
	
	glPopMatrix();
}

void IsoTasty::Renderer::drawTile(float x, float y, float z, float width, float height, float depth, float heights[4]) {
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	// vertex coords array
	float vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
						  1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
						  1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
						  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
						  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
						  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

	// normal array
	float normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
						 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
						 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
						 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
						 0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
						 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

	// color array
	float colors[] = {0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v1-v2-v3
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v3-v4-v5
						1,1,1,  0,1,1,  0,1,0,  1,1,0,                                   // v0-v5-v6-v1
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v1-v6-v7-v2
						0,0,0,  0,0,1,  1,0,1,  1,0,0,                                   // v7-v4-v3-v2
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0};// v4-v7-v6-v5

	unsigned char indices[] = {0,1,2,3,
						 4,5,6,7,
						 8,9,10,11,
						 12,13,14,15,
						 16,17,18,19,
						 20,21,22,23};

	// v0 (bottom-right)
	vertices[0*3+1] = heights[IsoTasty::BOT_RIGHT];
	vertices[4*3+1] = heights[IsoTasty::BOT_RIGHT];
	vertices[8*3+1] = heights[IsoTasty::BOT_RIGHT];
	
	// v1 (bottom-left)
	vertices[1*3+1] = heights[IsoTasty::BOT_LEFT];
	vertices[11*3+1] = heights[IsoTasty::BOT_LEFT];
	vertices[12*3+1] = heights[IsoTasty::BOT_LEFT];
	
	// v5 (top-right)
	vertices[7*3+1] = heights[IsoTasty::TOP_RIGHT];
	vertices[9*3+1] = heights[IsoTasty::TOP_RIGHT];
	vertices[23*3+1] = heights[IsoTasty::TOP_RIGHT];
	
	// v6 (top-left)
	vertices[10*3+1] = heights[IsoTasty::TOP_LEFT];
	vertices[13*3+1] = heights[IsoTasty::TOP_LEFT];
	vertices[22*3+1] = heights[IsoTasty::TOP_LEFT];

	glCullFace(GL_BACK);

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(width, height, depth);

	drawArrays(vertices, normals, colors, indices, 24);

	glPopMatrix();
}
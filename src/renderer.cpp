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
#include<glut.h>

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

void IsoTasty::Renderer::setProjection(unsigned int width, unsigned int height, bool orthographic, double rotation, double translationX, double translationZ, double zoom) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  
  zoom /= 2;

	double dist = sqrt(1 / 3.0);
	double aspect = (double)width / (double)height;

  double nearf = 1;
  double farf = 20.0;

  double fov = 3.1415926 / 4.0;

  double top = tan(fov * 0.5) * nearf;
  double bottom = -top;

  double left = aspect * bottom;
  double right = aspect * top;

  if (orthographic) {  
    glOrtho(left*4, right*4, bottom*4, top*4, nearf, farf);
    glTranslated(0, 0, -5);

    glRotated(35.264, 1.0, 0.0, 0.0);
  }
  else {
    glFrustum(left, right, bottom, top, nearf, farf);
    glTranslated(0, 0, -5);

    glRotated(35.264 * 2, 1.0, 0.0, 0.0);
  }
	glRotated(rotation, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslated(translationX, 1, translationZ);
	glScaled(zoom, zoom, zoom);
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

	glPushMatrix();
	glTranslatef(x, y, z);
  glScalef(width/2.0, height/2.0, depth/2.0);

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

void IsoTasty::Renderer::drawTileTop(float x, float y, float z, float width, float height, float depth, float heights[4], float firsts[4], float seconds[4]) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(width, height, depth);

	glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
	
	// Top
	glBegin(GL_LINE_LOOP);
	glVertex3f( 1.0f,  heights[IsoTasty::TOP_RIGHT], -1.0f);
	glVertex3f( 0.34f, firsts[IsoTasty::TOP_LEFT],   -1.0f);
	glVertex3f(-0.34f, seconds[IsoTasty::TOP_LEFT],  -1.0f);
	glVertex3f(-1.0f,  heights[IsoTasty::TOP_LEFT],  -1.0f);
	glVertex3f(-1.0f,  seconds[IsoTasty::BOT_LEFT],  -0.34f);
	glVertex3f(-1.0f,  firsts[IsoTasty::BOT_LEFT],    0.34f);
	glVertex3f(-1.0f,  heights[IsoTasty::BOT_LEFT],   1.0f);
	glVertex3f(-0.34f, seconds[IsoTasty::BOT_RIGHT],  1.0f);
	glVertex3f( 0.34f, firsts[IsoTasty::BOT_RIGHT],   1.0f);
	glVertex3f( 1.0f,  heights[IsoTasty::BOT_RIGHT],  1.0f);
	glVertex3f( 1.0f,  firsts[IsoTasty::TOP_RIGHT],   0.34f);
	glVertex3f( 1.0f,  seconds[IsoTasty::TOP_RIGHT], -0.34f);
	glEnd();
	
	glPopMatrix();
}

void IsoTasty::Renderer::drawTile(float x, float y, float z, float width, float height, float depth, float heights[4], float fy[4], float sy[4]) {
	// cube ///////////////////////////////////////////////////////////////////////
	// It is complicated...
	//
	//     v6-v7-v8-v9
	//  v5/|   v10/|  ----------v9
	// v4/ |  v11/ |  | v14 v15 | <-- Top square
	//  v3-v2-v1-v0|  | v13 v12 |
	//  |  |    |  |  ----------v0
	//  |  |v19-|--|v22
	//  | /v18  | /v23
	//  |/v17   |/v24
	//v16-v27-v26-v25

	// vertex coords array
	float vertices[] = {
     1.000000f, 1.000000f, 1.000000f,  1.000000f, 1.000000f, 0.333333f,  0.333333f, 1.000000f, 0.333333f,  0.333333f, 1.000000f, 1.000000f,
     0.333333f, 1.000000f, 1.000000f,  0.333333f, 1.000000f, 0.333333f, -0.333333f, 1.000000f, 0.333333f, -0.333333f, 1.000000f, 1.000000f,
    -0.333333f, 1.000000f, 1.000000f, -0.333333f, 1.000000f, 0.333333f, -1.000000f, 1.000000f, 0.333333f, -1.000000f, 1.000000f, 1.000000f,
    -0.333333f, 1.000000f, 0.333333f, -0.333333f, 1.000000f,-0.333333f, -1.000000f, 1.000000f,-0.333333f, -1.000000f, 1.000000f, 0.333333f,
    -0.333333f, 1.000000f,-0.333333f, -0.333333f, 1.000000f,-1.000000f, -1.000000f, 1.000000f,-1.000000f, -1.000000f, 1.000000f,-0.333333f,
     0.333333f, 1.000000f,-0.333333f,  0.333333f, 1.000000f,-1.000000f, -0.333333f, 1.000000f,-1.000000f, -0.333333f, 1.000000f,-0.333333f,
     1.000000f, 1.000000f,-0.333333f,  1.000000f, 1.000000f,-1.000000f,  0.333333f, 1.000000f,-1.000000f,  0.333333f, 1.000000f,-0.333333f,
     1.000000f, 1.000000f, 0.333333f,  1.000000f, 1.000000f,-0.333333f,  0.333333f, 1.000000f,-0.333333f,  0.333333f, 1.000000f, 0.333333f,
     0.333333f, 1.000000f, 0.333333f,  0.333333f, 1.000000f,-0.333333f, -0.333333f, 1.000000f,-0.333333f, -0.333333f, 1.000000f, 0.333333f,
     1.000000f,-1.000000f, 1.000000f,  1.000000f, 1.000000f, 1.000000f,  0.333333f, 1.000000f, 1.000000f,  0.333333f,-1.000000f, 1.000000f,
     0.333333f,-1.000000f, 1.000000f,  0.333333f, 1.000000f, 1.000000f, -0.333333f, 1.000000f, 1.000000f, -0.333333f,-1.000000f, 1.000000f,
    -0.333333f,-1.000000f, 1.000000f, -0.333333f, 1.000000f, 1.000000f, -1.000000f, 1.000000f, 1.000000f, -1.000000f,-1.000000f, 1.000000f,
    -1.000000f,-1.000000f, 1.000000f, -1.000000f, 1.000000f, 1.000000f, -1.000000f, 1.000000f, 0.333333f, -1.000000f,-1.000000f, 0.333333f,
    -1.000000f,-1.000000f, 0.333333f, -1.000000f, 1.000000f, 0.333333f, -1.000000f, 1.000000f,-0.333333f, -1.000000f,-1.000000f,-0.333333f,
    -1.000000f,-1.000000f,-0.333333f, -1.000000f, 1.000000f,-0.333333f, -1.000000f, 1.000000f,-1.000000f, -1.000000f,-1.000000f,-1.000000f,
    -1.000000f,-1.000000f,-1.000000f, -1.000000f, 1.000000f,-1.000000f, -0.333333f, 1.000000f,-1.000000f, -0.333333f,-1.000000f,-1.000000f,
    -0.333333f,-1.000000f,-1.000000f, -0.333333f, 1.000000f,-1.000000f,  0.333333f, 1.000000f,-1.000000f,  0.333333f,-1.000000f,-1.000000f,
     0.333333f,-1.000000f,-1.000000f,  0.333333f, 1.000000f,-1.000000f,  1.000000f, 1.000000f,-1.000000f,  1.000000f,-1.000000f,-1.000000f,
     1.000000f,-1.000000f,-1.000000f,  1.000000f, 1.000000f,-1.000000f,  1.000000f, 1.000000f,-0.333333f,  1.000000f,-1.000000f,-0.333333f,
     1.000000f,-1.000000f,-0.333333f,  1.000000f, 1.000000f,-0.333333f,  1.000000f, 1.000000f, 0.333333f,  1.000000f,-1.000000f, 0.333333f,
     1.000000f,-1.000000f, 0.333333f,  1.000000f, 1.000000f, 0.333333f,  1.000000f, 1.000000f, 1.000000f,  1.000000f,-1.000000f, 1.000000f,
  };

	// normal array
	float normals[] = {
    1, 1, 1, -1, 1, 1, -1,-1, 1,  1,-1, 1, // v0-v11-v12-v1
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v1-v12-v13-v2
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v2-v13-v4-v3
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v13-v14-v5-v4
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v14-v7-v6-v5
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v15-v8-v7-v14
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v10-v9-v8-v15
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v11-v10-v15-v12
    1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1, // v12-v15-v14-v13

    1, 1, 1,  1, 1,-1, -1, 1,-1, -1, 1, 1, // v25-v0-v1-v26
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v26-v1-v2-v27
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v27-v2-v3-v16
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v16-v3-v4-v17
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v17-v4-v5-v18
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v18-v5-v6-v19
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v19-v6-v7-v20
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v20-v7-v8-v21
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v21-v8-v9-v22
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v22-v9-v10-v23
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v23-v10-v11-v24
   -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1, 1, // v24-v11-v0-v25
  };

	// color array
	float colors[] = {    
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v0-v11-v12-v1
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v1-v12-v13-v2
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v2-v13-v4-v3
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v13-v14-v5-v4
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v14-v7-v6-v5
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v15-v8-v7-v14
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v10-v9-v8-v15
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v11-v10-v15-v12
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0, // v12-v15-v14-v13
    
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v25-v0-v1-v26
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v26-v1-v2-v27
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v27-v2-v3-v16
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v16-v3-v4-v17
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v17-v4-v5-v18
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v18-v5-v6-v19
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v19-v6-v7-v20
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v20-v7-v8-v21
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v21-v8-v9-v22
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v22-v9-v10-v23
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v23-v10-v11-v24
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0, // v24-v11-v0-v25
  };
	
	unsigned char lookup[] = {
    0,  11, 12,  1,
    1,  12, 13,  2,
    2,  13,  4,  3,
    13, 14,  5,  4,
    14,  7,  6,  5,
    15,  8,  7, 14,
    10,  9,  8, 15,
    11, 10, 15, 12,
    12, 15, 14, 13,
    
    25,  0,  1, 26,
    26,  1,  2, 27,
    27,  2,  3, 16,
    16,  3,  4, 17,
    17,  4,  5, 18,
    18,  5,  6, 19,
    19,  6,  7, 20,
    20,  7,  8, 21,
    21,  8,  9, 22,
    22,  9, 10, 23,
    23, 10, 11, 24,
    24, 11,  0, 25
  };

	unsigned char indices[sizeof(lookup)];
	for (int i = 0; i < sizeof(lookup); i++) {
		indices[i] = i;
	}

	float top_points[16];
	
	// v4 - v13 - v12 - v11
	top_points[3] = heights[IsoTasty::BOT_LEFT];
	top_points[4] = fy[IsoTasty::BOT_LEFT];
	top_points[5] = sy[IsoTasty::BOT_LEFT];
	top_points[6] = heights[IsoTasty::TOP_LEFT];

	// Interpolate the curve (v3-v4-v5-v6) across to (v0-v11-v10-v9)
	top_points[0] = heights[IsoTasty::BOT_RIGHT];
	top_points[11] = fy[IsoTasty::TOP_RIGHT];
	top_points[10] = sy[IsoTasty::TOP_RIGHT];
	top_points[9] = heights[IsoTasty::TOP_RIGHT];

	top_points[1] = fy[IsoTasty::BOT_RIGHT];
	top_points[2] = sy[IsoTasty::BOT_RIGHT];
	top_points[7] = sy[IsoTasty::TOP_LEFT];
	top_points[8] = fy[IsoTasty::TOP_LEFT];

	// Determine the relative height (of the point within the curve) of each inner point
	float curveStartHeight = abs(top_points[6] - top_points[3]);
	float curveStartPos = top_points[6];
	if (top_points[3] < top_points[6]) {
		curveStartPos = top_points[3];
	}
	float curveStartPtA = (top_points[4] - curveStartPos) / curveStartHeight;
	float curveStartPtB = (top_points[5] - curveStartPos) / curveStartHeight;
	if (curveStartHeight == 0) {
		curveStartPtA = 0;
		curveStartPtB = 0;
	}
	float curveEndHeight = abs(top_points[9] - top_points[0]);
	float curveEndPos = top_points[9];
	if (top_points[0] < top_points[9]) {
		curveEndPos = top_points[0];
	}
	float curveEndPtA = (top_points[11] - curveEndPos) / curveEndHeight;
	float curveEndPtB = (top_points[10] - curveEndPos) / curveEndHeight;
	if (curveEndHeight == 0) {
		curveEndPtA = 0;
		curveEndPtB = 0;
	}

	// Linear interpolation
	float curveMidPos1 = top_points[2];
	if (top_points[7] < top_points[2]) {
		curveMidPos1 = top_points[7];
	}
	float curveMidPos2 = top_points[1];
	if (top_points[8] < top_points[1]) {
		curveMidPos2 = top_points[8];
	}
	float deltaA = (curveStartPtA - curveEndPtA) / 3.0f;
	float deltaB = (curveStartPtB - curveEndPtB) / 3.0f;
	float curveMidAPt1 = curveStartPtA - deltaA;
	float curveMidAPt2 = curveEndPtA + deltaA;
	float curveMidBPt1 = curveStartPtB - deltaB;
	float curveMidBPt2 = curveEndPtB + deltaB;
	top_points[12] = abs(top_points[1] - top_points[8]) * curveMidAPt2 + curveMidPos2;
	top_points[13] = abs(top_points[2] - top_points[7]) * curveMidAPt1 + curveMidPos1;
	top_points[14] = abs(top_points[2] - top_points[7]) * curveMidBPt1 + curveMidPos1;
	top_points[15] = abs(top_points[1] - top_points[8]) * curveMidBPt2 + curveMidPos2;

	for (int i = 0; i < sizeof(lookup); i++) {
		unsigned int index = (i*3) + 1;
		if (lookup[i] < 16) {
			vertices[index] = top_points[lookup[i]];
		}
	}

	glCullFace(GL_BACK);

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(width, height, depth);

	drawArrays(vertices, normals, colors, indices, sizeof(lookup));

	glPopMatrix();
}
#include "iso-tasty/renderer.h"

#ifndef NO_GL
	#ifdef _WIN32
	#include "windows.h"
	#endif

	#include "GL/gl.h"
	#include "GL/glu.h"
#endif

#include "math.h"

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
	return true;
}

void IsoTasty::Renderer::setProjection(unsigned int width, unsigned int height, double rotation, double translationX, double translationZ) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double dist = sqrt(1 / 3.0);
	double aspect = (double)width/(double)height;
	glOrtho(-aspect*width/2.0, aspect*width/2.0, -aspect*height/2.0, aspect*height/2.0, -aspect*height*4, aspect*height*4);
	glRotated(35.264, 1.0, 0.0, 0.0);
	glRotated(rotation, 0.0, 1.0, 0.0);
	glScaled(32.0f, 32.0f, 32.0f);
	glTranslated(translationX, 0, translationZ);
	 
	glMatrixMode(GL_MODELVIEW);
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
	static const GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
						  1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
						  1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
						  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
						  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
						  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

	// normal array
	static const GLfloat normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
						 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
						 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
						 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
						 0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
						 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

	// color array
	static const GLfloat colors[] = {0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v1-v2-v3
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v0-v3-v4-v5
						1,1,1,  0,1,1,  0,1,0,  1,1,0,                                   // v0-v5-v6-v1
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0, // v1-v6-v7-v2
						0,0,0,  0,0,1,  1,0,1,  1,0,0,                                   // v7-v4-v3-v2
						0.72f,0.54f,0,  0.72f,0.54f,0,  0.52f,0.388f,0,  0.72f,0.388f,0};// v4-v7-v6-v5

	static const GLubyte indices[] = {0,1,2,3,
						 4,5,6,7,
						 8,9,10,11,
						 12,13,14,15,
						 16,17,18,19,
						 20,21,22,23};

	glCullFace(GL_BACK);

	float half_width = width / 2.0f;
	float half_height = height / 2.0f;
	float half_depth = depth / 2.0f;

	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(half_width, half_height, half_depth);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
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
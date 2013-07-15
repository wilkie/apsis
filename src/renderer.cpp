#include "apsis/renderer.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"
#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

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

/*Apsis::Primitives::VertexArray     vao;
Apsis::Primitives::VertexBuffer    vbo;
Apsis::Primitives::VertexShader    vs;
Apsis::Primitives::FragmentShader  fs;*/

Apsis::Renderer::Renderer() {
  /*vao = Primitives::VertexArray();
  vbo = Primitives::VertexBuffer();
  vs  = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");*/
  Apsis::Primitives::FragmentShader fs  = Primitives::FragmentShader::fromFile("src/shaders/fragment/colorize.glsl");
}

void Apsis::Renderer::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glColor3d(1.0, 1.0, 1.0);
}

bool Apsis::Renderer::initializeViewport(unsigned int width, unsigned int height) {
  return true;
  glClearColor(0, 0, 0, 0);
  glClearDepth(1.0f);
 
  glViewport(0, 0, width, height);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  return true;
}

void Apsis::Renderer::setProjection(unsigned int width, unsigned int height, bool orthographic, double rotation, double translationX, double translationZ, double zoom) {
  zoom /= 2;

  double dist = sqrt(1 / 3.0);
  float aspect = (double)width / (double)height;

  float nearf = 1;
  float farf = 20.0;

  float fov = 3.1415926 / 4.0;

  double top = tan(fov * 0.5) * nearf;
  double bottom = -top;

  double left = aspect * bottom;
  double right = aspect * top;

  glm::mat4 projection;

  if (orthographic) {
    projection = glm::ortho(left*4, right*4, bottom*4, top*4);
  }
  else {
    projection = glm::perspective(fov, aspect, nearf, farf);
  }

  glm::mat4 view = glm::rotate(
                     glm::translate(
                       glm::mat4(1.0f),
                       glm::vec3(0.0f, 0.0f, -5.0f)),
                     35.264f, glm::vec3(1.0f, 0.0f, 0.0f));

  if (!orthographic) {
    glm::rotate(view, 35.264f, glm::vec3(1.0f, 0.0f, 0.0f));
  }

  glm::scale(
    glm::translate(
      glm::rotate(view, (float)rotation, glm::vec3(0.0f, 1.0f, 0.0f)),
      glm::vec3(translationX, 0, translationZ)),
    glm::vec3(zoom, zoom, zoom));

  glm::mat4 model = glm::mat4(1.0f);
}

void Apsis::Renderer::drawArrays(const float vertices[], const float normals[], const float colors[], const unsigned char indices[], unsigned int num) {
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

void Apsis::Renderer::drawCube(float x, float y, float z, float width, float height, float depth) {
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

void Apsis::Renderer::test() {
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

void Apsis::Renderer::drawSphere(float x, float y, float z, float width, float height, float depth) {
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

void Apsis::Renderer::drawSquare(float x, float y, float z, float width, float depth) {
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

void Apsis::Renderer::drawTileTop(float x, float y, float z, float width, float height, float depth, float heights[4], float firsts[4], float seconds[4]) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(width, height, depth);

  glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
  
  // Top
  glBegin(GL_LINE_LOOP);
  glVertex3f( 1.0f,  heights[Apsis::TOP_RIGHT], -1.0f);
  glVertex3f( 0.34f,  firsts[Apsis::TOP_LEFT],  -1.0f);
  glVertex3f(-0.34f, seconds[Apsis::TOP_LEFT],  -1.0f);
  glVertex3f(-1.0f,  heights[Apsis::TOP_LEFT],  -1.0f);
  glVertex3f(-1.0f,  seconds[Apsis::BOT_LEFT],  -0.34f);
  glVertex3f(-1.0f,   firsts[Apsis::BOT_LEFT],   0.34f);
  glVertex3f(-1.0f,  heights[Apsis::BOT_LEFT],   1.0f);
  glVertex3f(-0.34f, seconds[Apsis::BOT_RIGHT],  1.0f);
  glVertex3f( 0.34f,  firsts[Apsis::BOT_RIGHT],  1.0f);
  glVertex3f( 1.0f,  heights[Apsis::BOT_RIGHT],  1.0f);
  glVertex3f( 1.0f,   firsts[Apsis::TOP_RIGHT],  0.34f);
  glVertex3f( 1.0f,  seconds[Apsis::TOP_RIGHT], -0.34f);
  glEnd();
  
  glPopMatrix();
}

void Apsis::Renderer::drawTile(float x, float y, float z, float width, float height, float depth, float heights[4], float fy[4], float sy[4]) {
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
     1.000000f, 1.000000f, 1.000000f, // v0
     0.333333f, 1.000000f, 1.000000f, // v1
    -0.333333f, 1.000000f, 1.000000f, // v2
    -1.000000f, 1.000000f, 1.000000f, // v3
    -1.000000f, 1.000000f, 0.333333f, // v4
    -1.000000f, 1.000000f,-0.333333f, // v5
    -1.000000f, 1.000000f,-1.000000f, // v6
    -0.333333f, 1.000000f,-1.000000f, // v7
     0.333333f, 1.000000f,-1.000000f, // v8
     1.000000f, 1.000000f,-1.000000f, // v9
     1.000000f, 1.000000f,-0.333333f, // v10
     1.000000f, 1.000000f, 0.333333f, // v11
     0.333333f, 1.000000f, 0.333333f, // v12
    -0.333333f, 1.000000f, 0.333333f, // v13
    -0.333333f, 1.000000f,-0.333333f, // v14
     0.333333f, 1.000000f,-0.333333f, // v15
    -1.000000f,-1.000000f, 1.000000f, // v16
    -1.000000f,-1.000000f, 0.333333f, // v17
    -1.000000f,-1.000000f,-0.333333f, // v18
    -1.000000f,-1.000000f,-1.000000f, // v19
    -0.333333f,-1.000000f,-1.000000f, // v20
     0.333333f,-1.000000f,-1.000000f, // v21
     1.000000f,-1.000000f,-1.000000f, // v22
     1.000000f,-1.000000f,-0.333333f, // v23
     1.000000f,-1.000000f, 0.333333f, // v24
     1.000000f,-1.000000f, 1.000000f, // v25
     0.333333f,-1.000000f, 1.000000f, // v26
    -0.333333f,-1.000000f, 1.000000f, // v27
  };

  // normal array
  float normals[] = {
     1.000000f, 1.000000f, 1.000000f, // v0
     0.333333f, 1.000000f, 1.000000f, // v1
    -0.333333f, 1.000000f, 1.000000f, // v2
    -1.000000f, 1.000000f, 1.000000f, // v3
    -1.000000f, 1.000000f, 0.333333f, // v4
    -1.000000f, 1.000000f,-0.333333f, // v5
    -1.000000f, 1.000000f,-1.000000f, // v6
    -0.333333f, 1.000000f,-1.000000f, // v7
     0.333333f, 1.000000f,-1.000000f, // v8
     1.000000f, 1.000000f,-1.000000f, // v9
     1.000000f, 1.000000f,-0.333333f, // v10
     1.000000f, 1.000000f, 0.333333f, // v11
     0.333333f, 1.000000f, 0.333333f, // v12
    -0.333333f, 1.000000f, 0.333333f, // v13
    -0.333333f, 1.000000f,-0.333333f, // v14
     0.333333f, 1.000000f,-0.333333f, // v15
    -1.000000f,-1.000000f, 1.000000f, // v16
    -1.000000f,-1.000000f, 0.333333f, // v17
    -1.000000f,-1.000000f,-0.333333f, // v18
    -1.000000f,-1.000000f,-1.000000f, // v19
    -0.333333f,-1.000000f,-1.000000f, // v20
     0.333333f,-1.000000f,-1.000000f, // v21
     1.000000f,-1.000000f,-1.000000f, // v22
     1.000000f,-1.000000f,-0.333333f, // v23
     1.000000f,-1.000000f, 0.333333f, // v24
     1.000000f,-1.000000f, 1.000000f, // v25
     0.333333f,-1.000000f, 1.000000f, // v26
    -0.333333f,-1.000000f, 1.000000f, // v27
  };

  // color array
  float colors[] = {    
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,
    0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,  0, 0.5, 0,
    
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0,
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0,
    0.72f, 0.54f, 0,  0.72f, 0.54f, 0,  0.52f, 0.388f, 0,  0.72f, 0.388f, 0,
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
  
  // v4 - v13 - v12 - v11
  vertices[(3 * 3) + 1] = heights[Apsis::BOT_LEFT];
  vertices[(4 * 3) + 1] = fy[Apsis::BOT_LEFT];
  vertices[(5 * 3) + 1] = sy[Apsis::BOT_LEFT];
  vertices[(6 * 3) + 1] = heights[Apsis::TOP_LEFT];

  // Interpolate the curve (v3-v4-v5-v6) across to (v0-v11-v10-v9)
  vertices[ (0 * 3) + 1] = heights[Apsis::BOT_RIGHT];
  vertices[(11 * 3) + 1] = fy[Apsis::TOP_RIGHT];
  vertices[(10 * 3) + 1] = sy[Apsis::TOP_RIGHT];
  vertices[ (9 * 3) + 1] = heights[Apsis::TOP_RIGHT];

  vertices[ (1 * 3) + 1] = fy[Apsis::BOT_RIGHT];
  vertices[ (2 * 3) + 1] = sy[Apsis::BOT_RIGHT];
  vertices[ (7 * 3) + 1] = sy[Apsis::TOP_LEFT];
  vertices[ (8 * 3) + 1] = fy[Apsis::TOP_LEFT];

  // Determine the relative height (of the point within the curve) of each inner point
  float curveStartHeight = abs(vertices[(6 * 3) + 1] - vertices[(3 * 3) + 1]);
  float curveStartPos = vertices[(6 * 3) + 1];
  if (vertices[(3 * 3) + 1] < vertices[(6 * 3) + 1]) {
    curveStartPos = vertices[(3 * 3) + 1];
  }
  float curveStartPtA = (vertices[(4 * 3) + 1] - curveStartPos) / curveStartHeight;
  float curveStartPtB = (vertices[(5 * 3) + 1] - curveStartPos) / curveStartHeight;
  if (curveStartHeight == 0) {
    curveStartPtA = 0;
    curveStartPtB = 0;
  }
  float curveEndHeight = abs(vertices[(9 * 3) + 1] - vertices[(0 * 3) + 1]);
  float curveEndPos = vertices[(9 * 3) + 1];
  if (vertices[(0 * 3) + 1] < vertices[(9 * 3) + 1]) {
    curveEndPos = vertices[(0 * 3) + 1];
  }
  float curveEndPtA = (vertices[(11 * 3) + 1] - curveEndPos) / curveEndHeight;
  float curveEndPtB = (vertices[(10 * 3) + 1] - curveEndPos) / curveEndHeight;
  if (curveEndHeight == 0) {
    curveEndPtA = 0;
    curveEndPtB = 0;
  }

  // Linear interpolation
  float curveMidPos1 = vertices[(2 * 3) + 1];
  if (vertices[(7 * 3) + 1] < vertices[(2 * 3) + 1]) {
    curveMidPos1 = vertices[(7 * 3) + 1];
  }
  float curveMidPos2 = vertices[(1 * 3) + 1];
  if (vertices[(8 * 3) + 1] < vertices[(1 * 3) + 1]) {
    curveMidPos2 = vertices[(8 * 3) + 1];
  }
  float deltaA = (curveStartPtA - curveEndPtA) / 3.0f;
  float deltaB = (curveStartPtB - curveEndPtB) / 3.0f;
  float curveMidAPt1 = curveStartPtA - deltaA;
  float curveMidAPt2 = curveEndPtA + deltaA;
  float curveMidBPt1 = curveStartPtB - deltaB;
  float curveMidBPt2 = curveEndPtB + deltaB;
  vertices[(12 * 3) + 1] = abs(vertices[(1 * 3) + 1] - vertices[(8 * 3) + 1]) * curveMidAPt2 + curveMidPos2;
  vertices[(13 * 3) + 1] = abs(vertices[(2 * 3) + 1] - vertices[(7 * 3) + 1]) * curveMidAPt1 + curveMidPos1;
  vertices[(14 * 3) + 1] = abs(vertices[(2 * 3) + 1] - vertices[(7 * 3) + 1]) * curveMidBPt1 + curveMidPos1;
  vertices[(15 * 3) + 1] = abs(vertices[(1 * 3) + 1] - vertices[(8 * 3) + 1]) * curveMidBPt2 + curveMidPos2;

  // Determine normals
  
  // v6--v7--v8--v9
  // v5-v14-v15-v10
  // v4-v13-v12-v11
  // v3--v2--v1--v0

  for (int i = 0; i < 16; i++) {
    //vectors[i] = glm::vec3(top_points[i]);
  }
  glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < sizeof(lookup); i++) {
    unsigned int index = (i*3) + 1;
    if (lookup[i] < 16) {
      // Determine normal of this vertex
    }
  }

  glCullFace(GL_BACK);

  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(width, height, depth);

  drawArrays(vertices, normals, colors, lookup, sizeof(lookup));

  glPopMatrix();
}
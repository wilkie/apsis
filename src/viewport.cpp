#include "apsis/viewport.h"

#include "stdio.h"
#include "math.h"

#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::Viewport::Viewport(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _rotation(0.0),
  _x(0.0),
  _z(0.0),
  _zoom(4.0) {

  Model::Thing thing = Model::Thing("resources/monkey.dae");

  _things.push_back(thing);
  _cameras.push_back(Primitives::Camera(glm::vec3(_x, 0, _z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 1.0, 0.5), 1.0));

  _terrain = new Apsis::World::Terrain(200, 200);

  this->move(0, 0);
}

unsigned int Apsis::Viewport::width() {
  return _width;
}

unsigned int Apsis::Viewport::height() {
  return _height;
}

float cubicBezier(float p0, float p1, float p2, float p3, float t) {
  float co1_1 = t;
  float co1_2 = t*t;
  float co1_3 = t*t*t;

  float co2_1 = (1-t);
  float co2_2 = (1-t)*(1-t);
  float co2_3 = (1-t)*(1-t)*(1-t);

  return co2_3*p3 + 3*co1_1*co2_2*p2 + 3*co1_2*co2_1*p1 + co1_3*p0;
}

void Apsis::Viewport::draw(Renderer* renderer, World::Terrain* map) {
  bool orthographic = false;
  double rotation = _rotation;
  double translationX = _x;
  double translationZ = _z;
  double zoom = _zoom;
  zoom /= 2;

  float dist = sqrtf(1.0f / 3.0f);
  float aspect = (float)_width / (float)_height;

  float nearf = 1;
  float farf = 20.0;

  float fov = 45.0f;

  float top = tanf(fov * 0.5f) * nearf;
  float bottom = -top;

  float left = aspect * bottom;
  float right = aspect * top;

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
    view = glm::rotate(view, 35.264f, glm::vec3(1.0f, 0.0f, 0.0f));
  } 

  view = glm::scale(
           glm::translate(
             glm::rotate(view, (float)rotation, glm::vec3(0.0f, 1.0f, 0.0f)),
             glm::vec3(translationX, 0, translationZ)),
           glm::vec3(zoom, zoom, zoom));

  //view = _cameras[0].view();

  glm::mat4 model = glm::mat4(1.0);

  //renderer->setProjection(_width, _height, false, _rotation, _x, _z, _zoom);
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_DEPTH_TEST);

  for (unsigned int i = 0; i < _things.size(); i++) {
    _things[i].draw(projection, _cameras[0], model);
  }

  this->_terrain->draw(projection, _cameras[0], model);

  return;

  /*
  

  Apsis::World::Tile* tile = map->atIndex(map->x(), map->z());
  renderer->drawSphere(
    (float)(map->x() - half_width - 0.5f), 
    (float)(-3 * (1 / _zoom) + (-tile->hover() + tile->cornerHeight(Apsis::TOP_LEFT))/2.0f),
    (float)(map->z() - half_height - 0.5f),
    0.25f, 0.25f, 0.25f);

  //renderer->test();*/
}

void Apsis::Viewport::move(double deltaX, double deltaZ) {
  double radians = _rotation / 180.0 * 3.1415926;
  double cosine = cos(radians);
  double sine = sin(radians);

  // Move respective of rotation
  _x -= deltaX*sine + deltaZ*cosine;
  _z += deltaX*cosine - deltaZ*sine;

  _cameras[0] = Primitives::Camera(glm::vec3(_x, _zoom, _z), glm::vec3(_x, 0.0, _z+5.0), glm::normalize(glm::vec3(cosine, 1.0, sine)),1.0);
}

void Apsis::Viewport::rotate(double delta) {
  if (delta < 0) {
    delta = -fmod(-delta, 360);
  }
  _rotation += 360.0;
  _rotation += delta;
  _rotation = fmod(_rotation, 360.0);

  this->move(0, 0);
}

void Apsis::Viewport::zoom(double factor) {
  _zoom /= factor;

  this->move(0, 0);
}
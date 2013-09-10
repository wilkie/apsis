#include "apsis/interface/viewport.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

using namespace Apsis;

Interface::Viewport::Viewport(const Apsis::Engine::Scene& scene,
                              float width,
                              float height)
  : _scene(scene),
    _camera(glm::vec2(0,0), 0.0f),
    _window(width / 2.0f, height / 2.0f, width, height, _draw) {
}

void Interface::Viewport::position(Apsis::Geometry::Point3d& point) const {
}

void Interface::Viewport::position(float x, float y, float z) const {
}

Geometry::Point3d Interface::Viewport::position() {
  Apsis::Geometry::Point3d point;
  point.x = _camera.eye().x;
  point.y = _camera.eye().y;
  point.z = _camera.eye().z;
  return point;
}

void Interface::Viewport::target(Apsis::Geometry::Point3d& point) const {
}

void Interface::Viewport::target(float x, float y, float z) const {
}

Geometry::Point3d Interface::Viewport::target() {
  Geometry::Point3d point;
  point.x = _camera.eye().x;
  point.y = _camera.eye().y;
  point.z = _camera.eye().z;
  return point;
}

void Interface::Viewport::draw() const {
  bool orthographic = true;

  float rotation = 0.0;

  float aspect = _window.position().width / _window.position().height;

  float nearf = 1;
  float farf = 20.0;

  float fov = 45.0f;

  float top = tanf(fov * 0.5f) * nearf;
  float bottom = -top;

  float left = aspect * bottom;
  float right = aspect * top;

  glm::mat4 projection;

  float half_height = _window.position().height / 2.0f;
  float half_width  = _window.position().width  / 2.0f;

  if (orthographic) {
    projection = glm::ortho(-half_width, half_width, -half_height, half_height);
  }
  else {
    projection = glm::perspective(fov, aspect, nearf, farf);
  }

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  float x = half_width;
  float z = half_height;

  float zoom = 1.0f;

  Primitives::Camera camera = Primitives::Camera(glm::vec2((float)(int)(x+0.5),
                                                           (float)(int)(z+0.5)),
                                                 zoom);

  _scene.scene().draw(projection, camera, glm::mat4(1.0f));
}

void Interface::Viewport::_draw(const Geometry::Rectangle& position,
                                       const World::Object& object) {
}
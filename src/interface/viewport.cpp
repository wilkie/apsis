#include "apsis/interface/viewport.h"

Apsis::Interface::Viewport::Viewport(Apsis::World::Scene& scene) 
  : _scene(&scene),
    _camera(glm::vec2(0,0), 0.0f) {
}

void Apsis::Interface::Viewport::position(Apsis::Geometry::Point3d& point) const {
}

void Apsis::Interface::Viewport::position(float x, float y, float z) const {
}

Apsis::Geometry::Point3d Apsis::Interface::Viewport::position() {
  Apsis::Geometry::Point3d point;
  point.x = _camera.eye().x;
  point.y = _camera.eye().y;
  point.z = _camera.eye().z;
  return point;
}

void Apsis::Interface::Viewport::target(Apsis::Geometry::Point3d& point) const {
}

void Apsis::Interface::Viewport::target(float x, float y, float z) const {
}

Apsis::Geometry::Point3d Apsis::Interface::Viewport::target() {
  Apsis::Geometry::Point3d point;
  point.x = _camera.eye().x;
  point.y = _camera.eye().y;
  point.z = _camera.eye().z;
  return point;
}

void Apsis::Interface::Viewport::draw() const {
}
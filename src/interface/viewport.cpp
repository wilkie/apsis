#include "apsis/interface/viewport.h"

#include "apsis/interface/label.h"
#include "apsis/interface/button.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

using namespace Apsis;

Interface::Viewport::Viewport(const Apsis::Engine::Scene& scene,
                              float width,
                              float height)
  : _scene(scene),
    _camera(*(Primitives::Vector2*)glm::value_ptr(glm::vec2(0,0)), 0.0f),
    _window(width / 2.0f, height / 2.0f, width, height, _init, _draw, _input, _motion, _update, _enter, _leave_) {
}

void Interface::Viewport::position(Apsis::Geometry::Point3d& point) const {
}

void Interface::Viewport::position(float x, float y, float z) const {
}

Geometry::Point3d Interface::Viewport::position() {
  Apsis::Geometry::Point3d point;
  point.x = _camera.eye().x();
  point.y = _camera.eye().y();
  point.z = _camera.eye().z();
  return point;
}

void Interface::Viewport::target(Apsis::Geometry::Point3d& point) const {
}

void Interface::Viewport::target(float x, float y, float z) const {
}

Geometry::Point3d Interface::Viewport::target() {
  Geometry::Point3d point;
  point.x = _camera.eye().x();
  point.y = _camera.eye().y();
  point.z = _camera.eye().z();
  return point;
}

void Interface::Viewport::draw(Apsis::Engine::Graphics& graphics) const {
  Primitives::Camera camera = Primitives::Camera(
    *(Primitives::Vector2*)glm::value_ptr(glm::vec2((float)(int)((_window.position().width/2.0f)+0.5),
                                                    (float)(int)((_window.position().height/2.0f)+0.5))),
    1.0f);

  graphics.orthographic();
  graphics.camera(camera);
  graphics.clear();

  _scene.scene().draw(graphics.projection(), camera);
}

void Interface::Viewport::_init(const Apsis::Interface::Window& window,
                                Apsis::World::Object& object) {
}

void Interface::Viewport::_draw(Apsis::Engine::Graphics& graphics,
                                const Apsis::Interface::Window& window,
                                const World::Object& object) {
}

void Interface::Viewport::_input(bool pressed,
                                 const Apsis::Input::Binding&    binding,
                                 const Apsis::Geometry::Point&   point,
                                 const Apsis::Interface::Window& window,
                                 Apsis::World::Object&           object) {
}

void Interface::Viewport::_update(float elapsed,
                                  Apsis::Geometry::Rectangle& position,
                                  Apsis::World::Object& object) {
}

void Interface::Viewport::_motion(const Apsis::Geometry::Point& point,
                                  const Apsis::Interface::Window& window,
                                  Apsis::World::Object& object) {
}

void Interface::Viewport::_enter(const Apsis::Geometry::Point& point,
                                 const Apsis::Interface::Window& window,
                                 Apsis::World::Object& object) {
}

void Interface::Viewport::_leave_(const Apsis::Geometry::Point& point,
                                  const Apsis::Interface::Window& window,
                                  Apsis::World::Object& object) {
}

const Apsis::Interface::Window& Apsis::Interface::Viewport::window() const {
  return _window;
}

Apsis::Interface::Window& Apsis::Interface::Viewport::window() {
  return _window;
}
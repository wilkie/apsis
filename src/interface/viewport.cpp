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
    _window(width / 2.0f, height / 2.0f, width, height, _init, _draw, _enter, _leave_) {

  Apsis::Interface::Window* foo = new Apsis::Interface::Window(500.0f, 500.0f, 100.0f, 100.0f,
                                                               Apsis::Interface::Button::init,
                                                               Apsis::Interface::Button::draw,
                                                               Apsis::Interface::Button::enter,
                                                               Apsis::Interface::Button::leave);
  Apsis::Interface::Window& meh = *foo;
  _window.add(meh);
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
/*

{
  "type": "interface",

  "windows": [
    {
      "widget": "label",
      "properties": {
        "text": "Current Item",
        "font": "Cinzel",
        "font-size": 16.0,
        "x": 70.0,
        "y": 10.0
      }
    },
    {
      "widget": "image",
      "properties": {
        "spritesheet": "hud",
        "spriteindex": 0,
        "x": 10.0,
        "y": 100.0,
        "width": 50.0,
        "height": 50.0
      }
    }
  ]
}

{
  "type": "widget",
  "name": "label",
  "inherit": true
}

{
  "type": "widget",
  "name": "foobar",
  "code": "foobar.rb"
}

module Foobar
  def onDraw
  end

  def onInputDown
  end

  def onInputUp
  end

  def onHover
  end
end

Themes can replace internal widgets? That is, a theme is basically
just a JSON description of widget types overriding those that
already exist. Defaults will be just built into the system or
just removed?

Interface JSON descriptions just refer to widget names. So widget
JSON descriptions can be replaced at will.

*/
void Interface::Viewport::draw(Apsis::Engine::Graphics& graphics) const {
  Primitives::Camera camera = Primitives::Camera(
    *(Primitives::Vector2*)glm::value_ptr(glm::vec2((float)(int)((_window.position().width/2.0f)+0.5),
                                                    (float)(int)((_window.position().height/2.0f)+0.5))),
    1.0f);

  graphics.orthographic();
  graphics.camera(camera);
  graphics.clear();

  _scene.scene().draw(graphics.projection(), camera);

  _window.draw(graphics);
}

void Interface::Viewport::_init(const Apsis::Geometry::Rectangle& position,
                                Apsis::World::Object& object) {
}

void Interface::Viewport::_draw(Apsis::Engine::Graphics& graphics,
                                const Geometry::Rectangle& position,
                                const World::Object& object) {
}

void Interface::Viewport::_enter(const Apsis::Geometry::Point& point,
                                 const Apsis::Geometry::Rectangle& position,
                                 const Apsis::World::Object& object) {
}

void Interface::Viewport::_leave_(const Apsis::Geometry::Point& point,
                                  const Apsis::Geometry::Rectangle& position,
                                  const Apsis::World::Object& object) {
}

const Apsis::Interface::Window& Apsis::Interface::Viewport::window() const {
  return _window;
}

Apsis::Interface::Window& Apsis::Interface::Viewport::window() {
  return _window;
}
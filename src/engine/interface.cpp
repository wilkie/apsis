#include "apsis/engine/interface.h"

using namespace Apsis;

std::vector<Engine::Interface*> Engine::Interface::_interface_engines;

Engine::Interface& Engine::Interface::basic() {
  Apsis::Engine::Interface* se = new Apsis::Engine::Interface();
  _interface_engines.push_back(se);
  return *se;
}

Engine::Interface::Interface()
  : _interface(NULL),
    _mainWindow(NULL) {
}

const Registry::Interface& Engine::Interface::iface() const {
  if (_interface == NULL) {
    throw "Engine::Interface Error: No interface has been loaded.";
  }
  return *_interface;
}

void Engine::Interface::iface(const Registry::Interface& iface) {
  _interface = &iface;
  _mainWindow = _interface->instance();
}

void Engine::Interface::update(float elapsed) {
  _mainWindow->update(elapsed);
}

void Engine::Interface::draw(Engine::Graphics& graphics) const {
  _mainWindow->draw(graphics);
}

void Engine::Interface::input(bool pressed,
                              const Apsis::Geometry::Point& point,
                              const Apsis::Input::Binding& binding) {
  _mainWindow->input(pressed, point, binding);
}

void Engine::Interface::motion(const Apsis::Geometry::Point& point) {
  _mainWindow->motion(point);
}

void Engine::Interface::enter(const Apsis::Geometry::Point& point) {
  _mainWindow->enter(point);
}

void Engine::Interface::leave(const Apsis::Geometry::Point& point) {
  _mainWindow->leave(point);
}
#include "apsis/engine/interface.h"

#include <algorithm>

using namespace Apsis;

std::vector<Engine::Interface*> Engine::Interface::_interface_engines;

Engine::Interface& Engine::Interface::basic(float width,
                                            float height) {
  Apsis::Engine::Interface* se = new Apsis::Engine::Interface(width,
                                                              height);
  _interface_engines.push_back(se);
  return *se;
}

Engine::Interface::Interface(float width,
                             float height)
  : _mainWindow(Engine::Object::basic(),
                width / 2.0f, height / 2.0f, width, height,
                Apsis::Interface::Event::defaultInit,
                Apsis::Interface::Event::defaultDraw,
                Apsis::Interface::Event::defaultInput,
                Apsis::Interface::Event::defaultMotion,
                Apsis::Interface::Event::defaultUpdate,
                Apsis::Interface::Event::defaultEnter,
                Apsis::Interface::Event::defaultLeave) {
}

const Registry::Interface& Engine::Interface::top() const {
  if (_interfaces.size() == 0) {
    throw "Engine::Interface::top() Error: No interface has been loaded.";
  }

  return *_interfaces[_interfaces.size()-1];
}

void Engine::Interface::push(const Registry::Interface& iface) {
  _interfaces.push_back(&iface);

  Apsis::Interface::Window* wnd = iface.instance(_mainWindow.position().width,
                                                 _mainWindow.position().height);

  _mainWindow.add(*wnd);
  _windows[iface.id()] = wnd;
}

void Engine::Interface::update(float elapsed) {
  _mainWindow.update(elapsed);
}

void Engine::Interface::draw(Engine::Graphics& graphics) const {
  _mainWindow.draw(graphics);
}

void Engine::Interface::input(bool pressed,
                              const Apsis::Geometry::Point& point,
                              const Apsis::Input::Binding& binding) {
  _mainWindow.input(pressed, point, binding);
}

void Engine::Interface::motion(const Apsis::Geometry::Point& point) {
  _mainWindow.motion(point);
}

void Engine::Interface::enter(const Apsis::Geometry::Point& point) {
  _mainWindow.enter(point);
}

void Engine::Interface::leave(const Apsis::Geometry::Point& point) {
  _mainWindow.leave(point);
}

// Convenience recursive function to deallocate a Window and all subwindows
static void _deleteWindow(const Apsis::Interface::Window* window) {
  if (window->childCount() > 0) {
    const Apsis::Interface::Window* start = &window->child();
    const Apsis::Interface::Window* current = start;

    do {
      const Apsis::Interface::Window* next = &current->next();
      _deleteWindow(current);
      current = next;
    } while (current != start);
  }

  delete window;
}

void Engine::Interface::pop() {
  const Registry::Interface& iface = *_interfaces[_interfaces.size()-1];
  Apsis::Interface::Window& wnd = *_windows[iface.id()];

  _interfaces.pop_back();
  _windows.erase(iface.id());

  // Detach
  wnd.detach();

  // Deallocate
  _deleteWindow(&wnd);
}

void Engine::Interface::pop(const Apsis::Registry::Interface& iface) {
  Apsis::Interface::Window& wnd = *_windows[iface.id()];
  _windows.erase(iface.id());

  std::vector<const Apsis::Registry::Interface*>::iterator it
    = std::find(_interfaces.begin(), _interfaces.end(), &iface);

  if (it != _interfaces.end()) {
    _interfaces.erase(it);
  }

  wnd.detach();

  _deleteWindow(&wnd);
}

void Engine::Interface::clear() {
  while (_interfaces.size() > 0) {
    pop();
  }
}

unsigned int Engine::Interface::count() const {
  return _interfaces.size();
}

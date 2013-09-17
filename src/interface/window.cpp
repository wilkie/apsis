#include "apsis/interface/window.h"

Apsis::Interface::Window::Window(float x,
                                 float y,
                                 float width,
                                 float height,
                                 InitEvent& init,
                                 DrawEvent& draw)
  : _position(x, y, width, height),
    _init(init),
    _draw(draw),
    _childCount(0),
    _child(NULL),
    _next(NULL),
    _prev(NULL) {

  // Call init method
  _init(_position, _object);
}

const Apsis::Geometry::Rectangle& Apsis::Interface::Window::position() const {
  return _position;
}

const Apsis::Interface::Window& Apsis::Interface::Window::next() const {
  return *_next;
}

const Apsis::Interface::Window& Apsis::Interface::Window::prev() const {
  return *_prev;
}

const Apsis::Interface::Window& Apsis::Interface::Window::child() const {
  return *_child;
}

unsigned int Apsis::Interface::Window::childCount() const {
  return _childCount;
}

void Apsis::Interface::Window::add(Apsis::Interface::Window& window) {
  // TODO: synchronize in some manner (can we do lockfree?)
  window._parent = this;

  if (_child == NULL) {
    // Insert as head
    _child = &window;
    window._next = &window;
    window._prev = &window;
  }
  else {
    // Insert into list
    Interface::Window* old_child = _child;

    _child = &window;
    _child->_next = old_child;
    _child->_prev = old_child->_prev;
    old_child->_prev = _child;
  }

  _childCount++;
}

void Apsis::Interface::Window::detach() {
  if (_parent == NULL) {
    // Not inserted or head...
    return;
  }

  if (_next == _prev) {
    _parent->_child = NULL;
  }
  else {
    _next->_prev = _prev;
    _prev->_next = _next;

    _next = NULL;
    _prev = NULL;
  }

  _parent->_childCount--;
  _parent = NULL;
}

bool Apsis::Interface::Window::attached() const {
  return _parent != NULL;
}

void Apsis::Interface::Window::draw(Apsis::Engine::Graphics& graphics) const {
  _draw(graphics, _position, _object);

  if (_child == NULL) {
    return;
  }

  // Draw children
  Interface::Window* current = _child;

  do  {
    current->draw(graphics);

    current = current->_next;
  } while (current != _child);
}

bool Apsis::Interface::Window::contains(float x, float y) const {
  Apsis::Geometry::Point p = {x, y};
  return _position.contains(&p);
}

const Apsis::Interface::Window& Apsis::Interface::Window::at(float x, float y) const {
  if (_child == NULL) {
    return *this;
  }

  Interface::Window* current = _child;

  // TODO: reverse direction since we want the order in which the widgets
  //       are seen and interacted (reverse to the order in which they
  //       are drawn.
  do  {
    if (current->contains(x, y)) {
      // Reposition point with local coordinates.
      float window_x = x - current->position().left();
      float window_y = y - current->position().top();

      // Recurse to the child window.
      return current->at(window_x, window_y);
    }

    current = current->_next;
  } while (current != _child);

  return *this;
}
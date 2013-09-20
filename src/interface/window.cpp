#include "apsis/interface/window.h"

using namespace Apsis;

Interface::Window::Window(float x,
                          float y,
                          float width,
                          float height,
                          Event::Init& init,
                          Event::Draw& draw,
                          Event::Input& input,
                          Event::Motion& motion,
                          Event::Update& update,
                          Event::Enter& enter,
                          Event::Leave& leave)
  : _position(x, y, width, height),
    _init(init),
    _draw(draw),
    _input(input),
    _motion(motion),
    _update(update),
    _enter(enter),
    _leave_(leave),
    _childCount(0),
    _child(NULL),
    _next(NULL),
    _prev(NULL),
    _updateInterval(0),
    _updateElapsed(0),
    _focus(NULL),
    _hover(NULL),
    _focused(false),
    _hovered(false) {

  // Call init method
  _init(*this, _object);
}

Interface::Window::Window(const Registry::Widget& widget,
                          float x,
                          float y,
                          float width,
                          float height)
  : _position(x, y, width, height),
    _init(widget.initEvent()),
    _draw(widget.drawEvent()),
    _input(widget.inputEvent()),
    _motion(widget.motionEvent()),
    _update(widget.updateEvent()),
    _enter(widget.enterEvent()),
    _leave_(widget.leaveEvent()),
    _childCount(0),
    _child(NULL),
    _next(NULL),
    _prev(NULL),
    _updateInterval(0),
    _updateElapsed(0),
    _focus(NULL),
    _hover(NULL),
    _focused(false),
    _hovered(false) {

  // Set properties
  for (unsigned int i = 0; i < widget.propertyCount(); i++) {
    const char* name = widget.propertyName(i);
    const char* def  = widget.propertyDefault(i);

    _object.set(name, def);
  }

  // Call init method
  _init(*this, _object);
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
    old_child->_prev->_next = _child;
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
  _draw(graphics, *this, _object);

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

void Apsis::Interface::Window::update(float elapsed) {
  if (_updateInterval > 0) {
    _updateElapsed += elapsed;

    while (_updateElapsed > _updateInterval) {
      _updateElapsed -= _updateInterval;
      _update(_updateInterval, _position, _object);
    }
  }

  if (_child == NULL) {
    return;
  }

  // TODO: We can pass up information about intervals to parents.
  //       With that we can decide we don't need to traverse further.

  // Update children
  Interface::Window* current = _child;

  do  {
    current->update(elapsed);

    current = current->_next;
  } while (current != _child);
}

void Apsis::Interface::Window::input(bool pressed,
                                     const Apsis::Geometry::Point& point,
                                     const Apsis::Input::Binding& binding) {
  // Input event for parent window
  _input(pressed, binding, point, *this, _object);

  if (binding.isMouse()) {
    // Pass along event to window under cursor if pointer event.

    // Find window under the pointer
    Interface::Window& window = childAt(point.x, point.y);

    if (&window != this) {
      // This window becomes focused window if a press
      if (pressed) {
        if (_focus != NULL) {
          _focus->_focused = false;
          // TODO: window unfocus event?
          _focus = NULL;
        }

        _focus = &window;
        window._focused = true;
        // TODO: window focus event?
      }

      // The event will be passed as a result of the normal passing
      // of input to focused child below.
    }
    else if (pressed) {
      // Do not send a mouse click to the focused window if there is
      // not a new focused window.
      return;
    }
  }

  // Pass along event to focused child window
  if (_focus != NULL) {
    _focus->input(pressed, point, binding);
  }
}

void Apsis::Interface::Window::motion(const Apsis::Geometry::Point& point) {
  // Is the pointer already hovered over us?
  if (!_hovered) {
    // We are now entered (sets _hovered to true)
    enter(point);
  }

  // Find window under the pointer
  Interface::Window& window = childAt(point.x, point.y);

  Apsis::Geometry::Point inner_point = point;
  inner_point.x -= _position.left();
  inner_point.y -= _position.top();

  // Pass the motion down
  if (&window != this) {
    window.motion(inner_point);

    // We should leave() the previously hovered child if hovering
    // a different child.
    if (_hover != NULL && _hover != &window) {
      _hover->leave(inner_point);
    }

    _hover = &window;
  }
  else {
    // We are not hovering over any of our children, so call leave for any
    // that were previously hovered.
    if (_hover != NULL) {
      Apsis::Geometry::Point inner_point = point;
      inner_point.x -= _position.left();
      inner_point.y -= _position.top();
      _hover->leave(inner_point);
      _hover = NULL;
    }
  }

  _motion(inner_point, *this, _object);
}

bool Apsis::Interface::Window::contains(float x, float y) const {
  Apsis::Geometry::Point p = {x, y};
  return _position.contains(&p);
}

bool Apsis::Interface::Window::focused() const {
  return _focused;
}

bool Apsis::Interface::Window::hovered() const {
  return _hovered;
}

const Apsis::Interface::Window& Apsis::Interface::Window::childAt(float x, float y) const {
  if (_child == NULL) {
    return *this;
  }

  Interface::Window* start   = _child->_prev;
  Interface::Window* current = start;

  do  {
    if (current->contains(x, y)) {
      return *current;
    }

    current = current->_prev;
  } while (current != start);

  return *this;
}

Apsis::Interface::Window& Apsis::Interface::Window::childAt(float x, float y) {
  if (_child == NULL) {
    return *this;
  }

  Interface::Window* start   = _child->_prev;
  Interface::Window* current = start;

  do  {
    if (current->contains(x, y)) {
      return *current;
    }

    current = current->_prev;
  } while (current != start);

  return *this;
}

const Apsis::Interface::Window& Apsis::Interface::Window::at(float x, float y) const {
  if (_child == NULL) {
    return *this;
  }

  Interface::Window* start   = _child->_prev;
  Interface::Window* current = start;

  do  {
    if (current->contains(x, y)) {
      // Reposition point with local coordinates.
      float window_x = x - current->position().left();
      float window_y = y - current->position().top();
       
      // Recurse to the child window.
      return current->at(window_x, window_y);
    }

    current = current->_prev;
  } while (current != start);

  return *this;
}

Apsis::Interface::Window& Apsis::Interface::Window::at(float x, float y) {
  if (_child == NULL) {
    return *this;
  }

  Interface::Window* start   = _child->_prev;
  Interface::Window* current = start;

  do  {
    if (current->contains(x, y)) {
      // Reposition point with local coordinates.
      float window_x = x - current->position().left();
      float window_y = y - current->position().top();
       
      // Recurse to the child window.
      return current->at(window_x, window_y);
    }

    current = current->_prev;
  } while (current != start);

  return *this;
}

void Apsis::Interface::Window::enter(const Apsis::Geometry::Point& point) {
  // We are being hovered over
  _hovered = true;

  // Event
  _enter(point, *this, _object);
}

void Apsis::Interface::Window::leave(const Apsis::Geometry::Point& point) {
  // No longer hovered over
  _hovered = false;

  // We also leave the hovered child, if there is one
  if (_hover != NULL) {
    Apsis::Geometry::Point inner_point = point;
    inner_point.x -= _position.left();
    inner_point.y -= _position.top();
    _hover->leave(inner_point);
    _hover = NULL;
  }

  // Event
  _leave_(point, *this, _object);
}
#include "apsis/input/input_engine.h"

Apsis::InputEngine::InputEngine() {
  _bindings = new Apsis::KeyBindingRepository();

#ifndef NO_SDL
  // Enable the joystick
  SDL_Joystick *joystick;

  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);
#endif
}

Apsis::InputEngine::~InputEngine() {
  delete _bindings;
}

Apsis::KeyBindingRepository* Apsis::InputEngine::keyBindings() {
  return _bindings;
}

bool Apsis::InputEngine::post(Apsis::Event& event, int& action) {
  if (event.type == Apsis::Event::Type::Quit) {
    // TODO: The QUIT action is so out of place.
    action = QUIT_EVENT;
    return true;
  }

  int event_id = 0;
  if (_bindings->yieldEvent(event.binding, event_id)) {
    if (event.pressed) {
      // TODO: Use std::set since held events are unique?
      if (!isEventHeld(event_id)) {
        _held.push_back(event_id);
      }
    }
    else {
      for (unsigned int i = 0; i < _held.size(); i++) {
        if (_held[i] == event_id) {
          _held.erase(_held.begin() + i);
          break;
        }
      }
    }

    action = event_id;
    return true;
  }

  return false;
}

bool Apsis::InputEngine::isEventHeld(int event) {
  for (unsigned int i = 0; i < _held.size(); i++) {
    if (_held[i] == event) {
      return true;
    }
  }
  return false;
}
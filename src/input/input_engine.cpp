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

int Apsis::InputEngine::post(Apsis::Event& event) {
  if (event.type == Apsis::Event::Type::Quit) {
    return QUIT_EVENT;
  }

  int event_id = _bindings->yieldEvent(event.binding);

  if (event_id == 0) {
    return 0;
  }

  if (event.pressed) {
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

  return event_id;
}

bool Apsis::InputEngine::isEventHeld(int event) {
  for (unsigned int i = 0; i < _held.size(); i++) {
    if (_held[i] == event) {
      return true;
    }
  }
  return false;
}
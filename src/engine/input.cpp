#include "apsis/engine/input.h"

#include <fstream>

std::vector<Apsis::Engine::Input*> Apsis::Engine::Input::_input_engines;

Apsis::Engine::Input& Apsis::Engine::Input::basic() {
  Apsis::Engine::Input* ie = new Apsis::Engine::Input();
  _input_engines.push_back(ie);
  return *ie;
}

Apsis::Engine::Input::Input() {
#ifndef NO_SDL
  // Enable the joystick
  SDL_Joystick *joystick;

  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);
#endif
}

bool Apsis::Engine::Input::press(const Apsis::Input::Binding& binding,
                                 unsigned int& action_id) {
  // TODO: We would need to remember individual keys that are held to do more complicated
  //       bindings. Later!
  unsigned int action_id_tmp = 0;
  if (Apsis::Registry::Action::yield(binding, action_id_tmp)) {
    // TODO: Use std::set since held events are unique?
    if (!isActionHeld(action_id_tmp)) {
      _held.insert(action_id_tmp);
    }

    action_id = action_id_tmp;
    return true;
  }

  return false;
}

bool Apsis::Engine::Input::release(const Apsis::Input::Binding& binding,
                                   unsigned int& action_id) {
  unsigned int action_id_tmp = 0;
  if (Apsis::Registry::Action::yield(binding, action_id_tmp)) {
    _held.erase(action_id_tmp);

    action_id = action_id_tmp;
    return true;
  }

  return false;
}

bool Apsis::Engine::Input::isActionHeld(unsigned int action_id) const {
  return _held.count(action_id) > 0;
}

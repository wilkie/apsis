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

int Apsis::InputEngine::forceEvent(bool pressed, KeyBinding* binding) {
  int event = _bindings->yieldEvent(binding);

  if (event == 0) {
    return 0;
  }

  if (pressed) {
    if (!isEventHeld(event)) {
      _held.push_back(event);
    }
  }
  else {
    for (unsigned int i = 0; i < _held.size(); i++) {
      if (_held[i] == event) {
        _held.erase(_held.begin() + i);
        break;
      }
    }
  }

  return event;
}

bool Apsis::InputEngine::isEventHeld(int event) {
  for (unsigned int i = 0; i < _held.size(); i++) {
    if (_held[i] == event) {
      return true;
    }
  }
  return false;
}

#ifndef NO_SDL
// OMG LONG FUNCTION JEEZ WEEZ CHEEZ WIZ (tm).
bool Apsis::InputEngine::_translateSDLKey(Apsis::KeyBinding* binding, SDL_Event* event) {
  binding->key = Apsis::Key::NONE;
  if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
    if (event->key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT)) {
      binding->shift = true;
    }
    if (event->key.keysym.mod & (KMOD_RCTRL | KMOD_LCTRL)) {
      binding->control = true;
    }
    if (event->key.keysym.mod & (KMOD_RALT | KMOD_LALT)) {
      binding->alt = true;
    }

    switch (event->key.keysym.sym) {
      case SDLK_LEFT:
        binding->key = Apsis::Key::LEFT;
        break;
      case SDLK_RIGHT:
        binding->key = Apsis::Key::RIGHT;
        break;
      case SDLK_UP:
        binding->key = Apsis::Key::UP;
        break;
      case SDLK_DOWN:
        binding->key = Apsis::Key::DOWN;
        break;
      case SDLK_a:
        binding->key = Apsis::Key::A;
        break;
      case SDLK_b:
        binding->key = Apsis::Key::B;
        break;
      case SDLK_c:
        binding->key = Apsis::Key::C;
        break;
      case SDLK_d:
        binding->key = Apsis::Key::D;
        break;
      case SDLK_e:
        binding->key = Apsis::Key::E;
        break;
      case SDLK_f:
        binding->key = Apsis::Key::F;
        break;
      case SDLK_g:
        binding->key = Apsis::Key::G;
        break;
      case SDLK_h:
        binding->key = Apsis::Key::H;
        break;
      case SDLK_i:
        binding->key = Apsis::Key::I;
        break;
      case SDLK_j:
        binding->key = Apsis::Key::J;
        break;
      case SDLK_k:
        binding->key = Apsis::Key::K;
        break;
      case SDLK_l:
        binding->key = Apsis::Key::L;
        break;
      case SDLK_m:
        binding->key = Apsis::Key::M;
        break;
      case SDLK_n:
        binding->key = Apsis::Key::N;
        break;
      case SDLK_o:
        binding->key = Apsis::Key::O;
        break;
      case SDLK_p:
        binding->key = Apsis::Key::P;
        break;
      case SDLK_q:
        binding->key = Apsis::Key::Q;
        break;
      case SDLK_r:
        binding->key = Apsis::Key::R;
        break;
      case SDLK_s:
        binding->key = Apsis::Key::S;
        break;
      case SDLK_t:
        binding->key = Apsis::Key::T;
        break;
      case SDLK_u:
        binding->key = Apsis::Key::U;
        break;
      case SDLK_v:
        binding->key = Apsis::Key::V;
        break;
      case SDLK_w:
        binding->key = Apsis::Key::W;
        break;
      case SDLK_x:
        binding->key = Apsis::Key::X;
        break;
      case SDLK_y:
        binding->key = Apsis::Key::Y;
        break;
      case SDLK_z:
        binding->key = Apsis::Key::Z;
        break;
      case SDLK_1:
        binding->key = Apsis::Key::NUM1;
        break;
      case SDLK_2:
        binding->key = Apsis::Key::NUM2;
        break;
      case SDLK_3:
        binding->key = Apsis::Key::NUM3;
        break;
      case SDLK_4:
        binding->key = Apsis::Key::NUM4;
        break;
      case SDLK_5:
        binding->key = Apsis::Key::NUM5;
        break;
      case SDLK_6:
        binding->key = Apsis::Key::NUM6;
        break;
      case SDLK_7:
        binding->key = Apsis::Key::NUM7;
        break;
      case SDLK_8:
        binding->key = Apsis::Key::NUM8;
        break;
      case SDLK_9:
        binding->key = Apsis::Key::NUM9;
        break;
      case SDLK_0:
        binding->key = Apsis::Key::NUM0;
        break;
      case SDLK_F1:
        binding->key = Apsis::Key::F1;
        break;
      case SDLK_F2:
        binding->key = Apsis::Key::F2;
        break;
      case SDLK_F3:
        binding->key = Apsis::Key::F3;
        break;
      case SDLK_F4:
        binding->key = Apsis::Key::F4;
        break;
      case SDLK_F5:
        binding->key = Apsis::Key::F5;
        break;
      case SDLK_F6:
        binding->key = Apsis::Key::F6;
        break;
      case SDLK_F7:
        binding->key = Apsis::Key::F7;
        break;
      case SDLK_F8:
        binding->key = Apsis::Key::F8;
        break;
      case SDLK_F9:
        binding->key = Apsis::Key::F9;
        break;
      case SDLK_F10:
        binding->key = Apsis::Key::F10;
        break;
      case SDLK_F11:
        binding->key = Apsis::Key::F11;
        break;
      case SDLK_F12:
        binding->key = Apsis::Key::F12;
        break;
      case SDLK_PERIOD:
        binding->key = Apsis::Key::PERIOD;
        break;
      case SDLK_COMMA:
        binding->key = Apsis::Key::COMMA;
        break;
      case SDLK_MINUS:
        binding->key = Apsis::Key::MINUS;
        break;
      case SDLK_EQUALS:
        binding->key = Apsis::Key::EQUALS;
        break;
      case SDLK_PLUS:
        binding->key = Apsis::Key::PLUS;
        break;
    }
  }
  else if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
    binding->key = (Apsis::Key::Code)(Apsis::Key::MOUSE_0 + event->button.button - 1);
  }
  else if (event->type == SDL_JOYBUTTONDOWN) {
    binding->key = (Apsis::Key::Code)(Apsis::Key::JOY_0 + event->jbutton.button);
  }
  else if (event->type == SDL_JOYAXISMOTION) {
    if (event->jaxis.axis == 0) {
      // Left-Right
      if (event->jaxis.value < 0) {
        binding->key = Apsis::Key::JOY_LEFT;
      }
      else {
        binding->key = Apsis::Key::JOY_RIGHT;
      }
    }
    else if (event->jaxis.axis == 1) {
      // Up-Down
      if (event->jaxis.value < 0) {
        binding->key = Apsis::Key::JOY_UP;
      }
      else {
        binding->key = Apsis::Key::JOY_DOWN;
      }
    }

    // TODO: Further axes ...
    return true;
  }
  else if (event->type == SDL_JOYHATMOTION) {
    // Hats are a bitmask that has a base of:
    int mask = Apsis::Key::JOY_POV_UP - 1;

    if (event->jhat.value & SDL_HAT_UP) {
      mask |= 0x1;
    }
    if (event->jhat.value & SDL_HAT_DOWN) {
      mask |= 0x2;
    }
    if (event->jhat.value & SDL_HAT_LEFT) {
      mask |= 0x4;
    }
    if (event->jhat.value & SDL_HAT_RIGHT) {
      mask |= 0x8;
    }

    binding->key = (Apsis::Key::Code)mask;
    return (mask && 0xf);
  }

  if (event->type == SDL_KEYDOWN ||
      event->type == SDL_MOUSEBUTTONDOWN ||
      event->type == SDL_JOYBUTTONDOWN) {

    return true;
  }

  return false;
}
#else
#endif

int Apsis::InputEngine::pollEvent() {
#ifndef NO_SDL
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return -1;
    }
    Apsis::KeyBinding binding = {Apsis::Key::NONE};
    bool pressed = _translateSDLKey(&binding, &event);

    return forceEvent(pressed, &binding);
  }
#endif

  // No event
  return 0;
}
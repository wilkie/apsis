#include "iso-tasty/input_engine.h"

IsoTasty::InputEngine::InputEngine() {
  _bindings = new IsoTasty::KeyBindingRepository();

#ifndef NO_SDL
  // Enable the joystick
  SDL_Joystick *joystick;

  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);
#endif
}

IsoTasty::InputEngine::~InputEngine() {
  delete _bindings;
}

IsoTasty::KeyBindingRepository* IsoTasty::InputEngine::keyBindings() {
  return _bindings;
}

int IsoTasty::InputEngine::forceEvent(bool pressed, KeyBinding* binding) {
  int event = _bindings->yieldEvent(binding);

  if (event == 0) {
    return 0;
  }

  if (pressed) {
    _held.push_back(event);
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

bool IsoTasty::InputEngine::isEventHeld(int event) {
  for (unsigned int i = 0; i < _held.size(); i++) {
    if (_held[i] == event) {
      return true;
    }
  }
  return false;
}

#ifndef NO_SDL
// OMG LONG FUNCTION JEEZ WEEZ CHEEZ WIZ (tm).
bool IsoTasty::InputEngine::_translateSDLKey(IsoTasty::KeyBinding* binding, SDL_Event* event) {
  binding->key = IsoTasty::Key::NONE;
  if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
    switch (event->key.keysym.sym) {
      case SDLK_LEFT:
        binding->key = IsoTasty::Key::LEFT;
        break;
      case SDLK_RIGHT:
        binding->key = IsoTasty::Key::RIGHT;
        break;
      case SDLK_UP:
        binding->key = IsoTasty::Key::UP;
        break;
      case SDLK_DOWN:
        binding->key = IsoTasty::Key::DOWN;
        break;
      case SDLK_a:
        binding->key = IsoTasty::Key::A;
        break;
      case SDLK_b:
        binding->key = IsoTasty::Key::B;
        break;
      case SDLK_c:
        binding->key = IsoTasty::Key::C;
        break;
      case SDLK_d:
        binding->key = IsoTasty::Key::D;
        break;
      case SDLK_e:
        binding->key = IsoTasty::Key::E;
        break;
      case SDLK_f:
        binding->key = IsoTasty::Key::F;
        break;
      case SDLK_g:
        binding->key = IsoTasty::Key::G;
        break;
      case SDLK_h:
        binding->key = IsoTasty::Key::H;
        break;
      case SDLK_i:
        binding->key = IsoTasty::Key::I;
        break;
      case SDLK_j:
        binding->key = IsoTasty::Key::J;
        break;
      case SDLK_k:
        binding->key = IsoTasty::Key::K;
        break;
      case SDLK_l:
        binding->key = IsoTasty::Key::L;
        break;
      case SDLK_m:
        binding->key = IsoTasty::Key::M;
        break;
      case SDLK_n:
        binding->key = IsoTasty::Key::N;
        break;
      case SDLK_o:
        binding->key = IsoTasty::Key::O;
        break;
      case SDLK_p:
        binding->key = IsoTasty::Key::P;
        break;
      case SDLK_q:
        binding->key = IsoTasty::Key::Q;
        break;
      case SDLK_r:
        binding->key = IsoTasty::Key::R;
        break;
      case SDLK_s:
        binding->key = IsoTasty::Key::S;
        break;
      case SDLK_t:
        binding->key = IsoTasty::Key::T;
        break;
      case SDLK_u:
        binding->key = IsoTasty::Key::U;
        break;
      case SDLK_v:
        binding->key = IsoTasty::Key::V;
        break;
      case SDLK_w:
        binding->key = IsoTasty::Key::W;
        break;
      case SDLK_x:
        binding->key = IsoTasty::Key::X;
        break;
      case SDLK_y:
        binding->key = IsoTasty::Key::Y;
        break;
      case SDLK_z:
        binding->key = IsoTasty::Key::Z;
        break;
      case SDLK_1:
        binding->key = IsoTasty::Key::NUM1;
        break;
      case SDLK_2:
        binding->key = IsoTasty::Key::NUM2;
        break;
      case SDLK_3:
        binding->key = IsoTasty::Key::NUM3;
        break;
      case SDLK_4:
        binding->key = IsoTasty::Key::NUM4;
        break;
      case SDLK_5:
        binding->key = IsoTasty::Key::NUM5;
        break;
      case SDLK_6:
        binding->key = IsoTasty::Key::NUM6;
        break;
      case SDLK_7:
        binding->key = IsoTasty::Key::NUM7;
        break;
      case SDLK_8:
        binding->key = IsoTasty::Key::NUM8;
        break;
      case SDLK_9:
        binding->key = IsoTasty::Key::NUM9;
        break;
      case SDLK_0:
        binding->key = IsoTasty::Key::NUM0;
        break;
      case SDLK_F1:
        binding->key = IsoTasty::Key::F1;
        break;
      case SDLK_F2:
        binding->key = IsoTasty::Key::F2;
        break;
      case SDLK_F3:
        binding->key = IsoTasty::Key::F3;
        break;
      case SDLK_F4:
        binding->key = IsoTasty::Key::F4;
        break;
      case SDLK_F5:
        binding->key = IsoTasty::Key::F5;
        break;
      case SDLK_F6:
        binding->key = IsoTasty::Key::F6;
        break;
      case SDLK_F7:
        binding->key = IsoTasty::Key::F7;
        break;
      case SDLK_F8:
        binding->key = IsoTasty::Key::F8;
        break;
      case SDLK_F9:
        binding->key = IsoTasty::Key::F9;
        break;
      case SDLK_F10:
        binding->key = IsoTasty::Key::F10;
        break;
      case SDLK_F11:
        binding->key = IsoTasty::Key::F11;
        break;
      case SDLK_F12:
        binding->key = IsoTasty::Key::F12;
        break;
    }
  }
  else if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
    binding->key = (IsoTasty::Key::Code)(IsoTasty::Key::MOUSE_0 + event->button.button - 1);
  }
  else if (event->type == SDL_JOYBUTTONDOWN) {
    binding->key = (IsoTasty::Key::Code)(IsoTasty::Key::JOY_0 + event->jbutton.button);
  }
  else if (event->type == SDL_JOYAXISMOTION) {
    if (event->jaxis.axis == 0) {
      // Left-Right
      if (event->jaxis.value < 0) {
        binding->key = IsoTasty::Key::JOY_LEFT;
      }
      else {
        binding->key = IsoTasty::Key::JOY_RIGHT;
      }
    }
    else if (event->jaxis.axis == 1) {
      // Up-Down
      if (event->jaxis.value < 0) {
        binding->key = IsoTasty::Key::JOY_UP;
      }
      else {
        binding->key = IsoTasty::Key::JOY_DOWN;
      }
    }

    // TODO: Further axes ...
    return true;
  }
  else if (event->type == SDL_JOYHATMOTION) {
    // Hats are a bitmask that has a base of:
    int mask = IsoTasty::Key::JOY_POV_UP - 1;

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

    binding->key = (IsoTasty::Key::Code)mask;
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

int IsoTasty::InputEngine::pollEvent() {
#ifndef NO_SDL
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return -1;
    }
    IsoTasty::KeyBinding binding = {IsoTasty::Key::NONE};
    bool pressed = _translateSDLKey(&binding, &event);

    return forceEvent(pressed, &binding);
  }
#endif

  // No event
  return 0;
}
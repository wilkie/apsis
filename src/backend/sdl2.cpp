#include "apsis/backend/sdl.h"
#include "apsis/sync/clock.h"

#include "apsis/engine/event.h"

#include "apsis/input/key.h"

#include "math.h"

#ifndef NO_SDL

static SDL_Window* _window;
static SDL_GLContext _context;

static void _translateSDLPosition(float& x, float& y, const SDL_Event& event) {
  // Get mouse position or joystick pressure

  if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
    // Mouse position
    x = (float)event.button.x;
    y = (float)event.button.y;
  }
  else if (event.type == SDL_JOYBUTTONDOWN) {
    // Pressure sensitivity?
  }
  else if (event.type == SDL_JOYAXISMOTION) {
    // Axis motion
    x = (float)((int)event.jaxis.value + 32768) / (32768.0f*2.0f);
  }
  else if (event.type == SDL_JOYBALLMOTION) {
    // Ball motion
    x = (float)(event.jball.xrel);
    y = (float)(event.jball.yrel);
  }
}

// OMG LONG FUNCTION JEEZ WEEZ CHEEZ WIZ (tm).
static bool _translateSDL_SCANCODEey(Apsis::Input::Binding& binding, const SDL_Event& event, bool& pressed) {
  if (event.type == SDL_KEYDOWN ||
      event.type == SDL_MOUSEBUTTONDOWN ||
      event.type == SDL_JOYBUTTONDOWN) {

    pressed = true;
  }
  else {
    pressed = false;
  }

  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
    binding.shift((event.key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT)) > 0);
    binding.control((event.key.keysym.mod & (KMOD_RCTRL | KMOD_LCTRL)) > 0);
    binding.alt((event.key.keysym.mod & (KMOD_RALT | KMOD_LALT)) > 0);

    switch (event.key.keysym.scancode) {
      case SDL_SCANCODE_LEFT:
        binding.key(Apsis::Key::LEFT);
        break;
      case SDL_SCANCODE_RIGHT:
        binding.key(Apsis::Key::RIGHT);
        break;
      case SDL_SCANCODE_UP:
        binding.key(Apsis::Key::UP);
        break;
      case SDL_SCANCODE_DOWN:
        binding.key(Apsis::Key::DOWN);
        break;
      case SDL_SCANCODE_A:
        binding.key(Apsis::Key::A);
        break;
      case SDL_SCANCODE_B:
        binding.key(Apsis::Key::B);
        break;
      case SDL_SCANCODE_C:
        binding.key(Apsis::Key::C);
        break;
      case SDL_SCANCODE_D:
        binding.key(Apsis::Key::D);
        break;
      case SDL_SCANCODE_E:
        binding.key(Apsis::Key::E);
        break;
      case SDL_SCANCODE_F:
        binding.key(Apsis::Key::F);
        break;
      case SDL_SCANCODE_G:
        binding.key(Apsis::Key::G);
        break;
      case SDL_SCANCODE_H:
        binding.key(Apsis::Key::H);
        break;
      case SDL_SCANCODE_I:
        binding.key(Apsis::Key::I);
        break;
      case SDL_SCANCODE_J:
        binding.key(Apsis::Key::J);
        break;
      case SDL_SCANCODE_K:
        binding.key(Apsis::Key::K);
        break;
      case SDL_SCANCODE_L:
        binding.key(Apsis::Key::L);
        break;
      case SDL_SCANCODE_M:
        binding.key(Apsis::Key::M);
        break;
      case SDL_SCANCODE_N:
        binding.key(Apsis::Key::N);
        break;
      case SDL_SCANCODE_O:
        binding.key(Apsis::Key::O);
        break;
      case SDL_SCANCODE_P:
        binding.key(Apsis::Key::P);
        break;
      case SDL_SCANCODE_Q:
        binding.key(Apsis::Key::Q);
        break;
      case SDL_SCANCODE_R:
        binding.key(Apsis::Key::R);
        break;
      case SDL_SCANCODE_S:
        binding.key(Apsis::Key::S);
        break;
      case SDL_SCANCODE_T:
        binding.key(Apsis::Key::T);
        break;
      case SDL_SCANCODE_U:
        binding.key(Apsis::Key::U);
        break;
      case SDL_SCANCODE_V:
        binding.key(Apsis::Key::V);
        break;
      case SDL_SCANCODE_W:
        binding.key(Apsis::Key::W);
        break;
      case SDL_SCANCODE_X:
        binding.key(Apsis::Key::X);
        break;
      case SDL_SCANCODE_Y:
        binding.key(Apsis::Key::Y);
        break;
      case SDL_SCANCODE_Z:
        binding.key(Apsis::Key::Z);
        break;
      case SDL_SCANCODE_1:
        binding.key(Apsis::Key::NUM1);
        break;
      case SDL_SCANCODE_2:
        binding.key(Apsis::Key::NUM2);
        break;
      case SDL_SCANCODE_3:
        binding.key(Apsis::Key::NUM3);
        break;
      case SDL_SCANCODE_4:
        binding.key(Apsis::Key::NUM4);
        break;
      case SDL_SCANCODE_5:
        binding.key(Apsis::Key::NUM5);
        break;
      case SDL_SCANCODE_6:
        binding.key(Apsis::Key::NUM6);
        break;
      case SDL_SCANCODE_7:
        binding.key(Apsis::Key::NUM7);
        break;
      case SDL_SCANCODE_8:
        binding.key(Apsis::Key::NUM8);
        break;
      case SDL_SCANCODE_9:
        binding.key(Apsis::Key::NUM9);
        break;
      case SDL_SCANCODE_0:
        binding.key(Apsis::Key::NUM0);
        break;
      case SDL_SCANCODE_F1:
        binding.key(Apsis::Key::F1);
        break;
      case SDL_SCANCODE_F2:
        binding.key(Apsis::Key::F2);
        break;
      case SDL_SCANCODE_F3:
        binding.key(Apsis::Key::F3);
        break;
      case SDL_SCANCODE_F4:
        binding.key(Apsis::Key::F4);
        break;
      case SDL_SCANCODE_F5:
        binding.key(Apsis::Key::F5);
        break;
      case SDL_SCANCODE_F6:
        binding.key(Apsis::Key::F6);
        break;
      case SDL_SCANCODE_F7:
        binding.key(Apsis::Key::F7);
        break;
      case SDL_SCANCODE_F8:
        binding.key(Apsis::Key::F8);
        break;
      case SDL_SCANCODE_F9:
        binding.key(Apsis::Key::F9);
        break;
      case SDL_SCANCODE_F10:
        binding.key(Apsis::Key::F10);
        break;
      case SDL_SCANCODE_F11:
        binding.key(Apsis::Key::F11);
        break;
      case SDL_SCANCODE_F12:
        binding.key(Apsis::Key::F12);
        break;
      case SDL_SCANCODE_PERIOD:
        binding.key(Apsis::Key::PERIOD);
        break;
      case SDL_SCANCODE_COMMA:
        binding.key(Apsis::Key::COMMA);
        break;
      case SDL_SCANCODE_MINUS:
        binding.key(Apsis::Key::MINUS);
        break;
      case SDL_SCANCODE_EQUALS:
        binding.key(Apsis::Key::EQUALS);
        break;
      case SDL_SCANCODE_KP_PLUS:
        // TODO: Keypad ??
        binding.key(Apsis::Key::PLUS);
        break;
      case SDL_SCANCODE_LCTRL:
        binding.key(Apsis::Key::LEFTCONTROL);
        break;
      case SDL_SCANCODE_RCTRL:
        binding.key(Apsis::Key::RIGHTCONTROL);
        break;
      case SDL_SCANCODE_LALT:
        binding.key(Apsis::Key::LEFTALT);
        break;
      case SDL_SCANCODE_RALT:
        binding.key(Apsis::Key::RIGHTALT);
        break;
    }
  }
  else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
    binding.key((Apsis::Key::Code)(Apsis::Key::MOUSE_0 + event.button.button - 1));
  }
  else if (event.type == SDL_MOUSEWHEEL) {
    //event.wheel.x;
    //event.wheel.y;
  }
  else if (event.type == SDL_JOYBUTTONDOWN) {
    binding.key((Apsis::Key::Code)(Apsis::Key::JOY_0 + event.jbutton.button));
  }
  else if (event.type == SDL_JOYAXISMOTION) {
    if (event.jaxis.axis == 0) {
      // Left-Right
      if (event.jaxis.value < 0) {
        binding.key(Apsis::Key::JOY_LEFT);
      }
      else {
        binding.key(Apsis::Key::JOY_RIGHT);
      }
    }
    else if (event.jaxis.axis == 1) {
      // Up-Down
      if (event.jaxis.value < 0) {
        binding.key(Apsis::Key::JOY_UP);
      }
      else {
        binding.key(Apsis::Key::JOY_DOWN);
      }
    }

    // TODO: Further axes ...
    pressed = true;
  }
  else if (event.type == SDL_JOYHATMOTION) {
    // Hats are a bitmask that has a base of:
    int mask = Apsis::Key::JOY_POV_UP - 1;

    if (event.jhat.value & SDL_HAT_UP) {
      mask |= 0x1;
    }
    if (event.jhat.value & SDL_HAT_DOWN) {
      mask |= 0x2;
    }
    if (event.jhat.value & SDL_HAT_LEFT) {
      mask |= 0x4;
    }
    if (event.jhat.value & SDL_HAT_RIGHT) {
      mask |= 0x8;
    }

    binding.key((Apsis::Key::Code)mask);

    // TODO: Double check wtf this is:
    pressed = (mask & 0xf) > 0;
  }
  else {
    return false;
  }

  return true;
}
#else
#endif

Apsis::Backend::Sdl::Sdl(const Apsis::Settings::Video& videoSettings)
  : _video(videoSettings) {
  if (!_initialize()) {
    throw "Cannot initialize backend.";
  }
}

void Apsis::Backend::Sdl::swap() {
#ifndef NO_SDL
  SDL_GL_SwapWindow(_window);
  SDL_Delay(1);
#endif
}

bool Apsis::Backend::Sdl::poll(Apsis::Engine::Event& event) {
#ifndef NO_SDL
  SDL_Event sdl_event;

  if (SDL_PollEvent(&sdl_event)) {
    if(sdl_event.type == SDL_QUIT) {
      event.systemEvent(Apsis::Engine::Event::SystemEvent::Quit);
      return true;
    }

    // TODO: Get rid of this need for constructing a fake binding!
    //       Do that and we never need a mutable Binding.
    Apsis::Input::Binding binding(Apsis::Key::A, false, false, false);
    bool pressed = true;
    if (sdl_event.type == SDL_MOUSEMOTION) {
      float x = (float)sdl_event.motion.x;
      float y = (float)sdl_event.motion.y;
      event.motion(x, y);
      return true;
    }
    else if (_translateSDL_SCANCODEey(binding, sdl_event, pressed)) {
      float x, y;
      _translateSDLPosition(x, y, sdl_event);
      event.binding(binding, x, y, pressed);
      return true;
    }
    return false;
  }
#endif
  return false;
}

void Apsis::Backend::Sdl::quit() {
#ifndef NO_SDL
  // Uninitialize the joystick
  //joystick = SDL_JoystickOpen(0);

  // Uninitialize the timer
  SDL_QuitSubSystem(SDL_INIT_TIMER);

  SDL_GL_DeleteContext(_context);
  SDL_DestroyWindow(_window);

  // Destruct SDL
  SDL_Quit();
#endif
}

bool Apsis::Backend::Sdl::_initialize() {
  if (_start()) {
    return true;
  }

  fprintf(stderr, "SDL backend cannot initialize.");
  return false;
}

bool Apsis::Backend::Sdl::_start() {
#ifdef NO_SDL
  return false;
#else
  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  // Initialize joystick by querying for the first joystick attached.
  SDL_Joystick *joystick;

  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);

  _window = SDL_CreateWindow("Apsis",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             _video.resolutionX,
                             _video.resolutionY,
                             SDL_WINDOW_SHOWN |
                             SDL_WINDOW_OPENGL);

  if (_window == NULL) {
    fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
    return false;
  }

  _context = SDL_GL_CreateContext(_window);

  SDL_GL_SetSwapInterval(1);

  if (SDL_InitSubSystem(SDL_INIT_TIMER) < 0) {
    // Error
    char error[1024];
    sprintf(error, "Unable to initialize SDL: SDL_InitSubSystem(SDL_INIT_TIMER) failed: %s\n", SDL_GetError());
    throw error;
  }

#ifndef JS_MODE
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();
#endif

  return true;
#endif
}

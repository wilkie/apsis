#include "apsis/backend/sdl.h"
#include "apsis/sync/clock.h"

#include "apsis/engine/event.h"

#include "apsis/input/key.h"

#include "math.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_SDL
// OMG LONG FUNCTION JEEZ WEEZ CHEEZ WIZ (tm).
static bool _translateSDLKey(Apsis::Input::Binding& binding, const SDL_Event& event, bool& pressed) {
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

    switch (event.key.keysym.sym) {
      case SDLK_LEFT:
        binding.key(Apsis::Key::LEFT);
        break;
      case SDLK_RIGHT:
        binding.key(Apsis::Key::RIGHT);
        break;
      case SDLK_UP:
        binding.key(Apsis::Key::UP);
        break;
      case SDLK_DOWN:
        binding.key(Apsis::Key::DOWN);
        break;
      case SDLK_a:
        binding.key(Apsis::Key::A);
        break;
      case SDLK_b:
        binding.key(Apsis::Key::B);
        break;
      case SDLK_c:
        binding.key(Apsis::Key::C);
        break;
      case SDLK_d:
        binding.key(Apsis::Key::D);
        break;
      case SDLK_e:
        binding.key(Apsis::Key::E);
        break;
      case SDLK_f:
        binding.key(Apsis::Key::F);
        break;
      case SDLK_g:
        binding.key(Apsis::Key::G);
        break;
      case SDLK_h:
        binding.key(Apsis::Key::H);
        break;
      case SDLK_i:
        binding.key(Apsis::Key::I);
        break;
      case SDLK_j:
        binding.key(Apsis::Key::J);
        break;
      case SDLK_k:
        binding.key(Apsis::Key::K);
        break;
      case SDLK_l:
        binding.key(Apsis::Key::L);
        break;
      case SDLK_m:
        binding.key(Apsis::Key::M);
        break;
      case SDLK_n:
        binding.key(Apsis::Key::N);
        break;
      case SDLK_o:
        binding.key(Apsis::Key::O);
        break;
      case SDLK_p:
        binding.key(Apsis::Key::P);
        break;
      case SDLK_q:
        binding.key(Apsis::Key::Q);
        break;
      case SDLK_r:
        binding.key(Apsis::Key::R);
        break;
      case SDLK_s:
        binding.key(Apsis::Key::S);
        break;
      case SDLK_t:
        binding.key(Apsis::Key::T);
        break;
      case SDLK_u:
        binding.key(Apsis::Key::U);
        break;
      case SDLK_v:
        binding.key(Apsis::Key::V);
        break;
      case SDLK_w:
        binding.key(Apsis::Key::W);
        break;
      case SDLK_x:
        binding.key(Apsis::Key::X);
        break;
      case SDLK_y:
        binding.key(Apsis::Key::Y);
        break;
      case SDLK_z:
        binding.key(Apsis::Key::Z);
        break;
      case SDLK_1:
        binding.key(Apsis::Key::NUM1);
        break;
      case SDLK_2:
        binding.key(Apsis::Key::NUM2);
        break;
      case SDLK_3:
        binding.key(Apsis::Key::NUM3);
        break;
      case SDLK_4:
        binding.key(Apsis::Key::NUM4);
        break;
      case SDLK_5:
        binding.key(Apsis::Key::NUM5);
        break;
      case SDLK_6:
        binding.key(Apsis::Key::NUM6);
        break;
      case SDLK_7:
        binding.key(Apsis::Key::NUM7);
        break;
      case SDLK_8:
        binding.key(Apsis::Key::NUM8);
        break;
      case SDLK_9:
        binding.key(Apsis::Key::NUM9);
        break;
      case SDLK_0:
        binding.key(Apsis::Key::NUM0);
        break;
      case SDLK_F1:
        binding.key(Apsis::Key::F1);
        break;
      case SDLK_F2:
        binding.key(Apsis::Key::F2);
        break;
      case SDLK_F3:
        binding.key(Apsis::Key::F3);
        break;
      case SDLK_F4:
        binding.key(Apsis::Key::F4);
        break;
      case SDLK_F5:
        binding.key(Apsis::Key::F5);
        break;
      case SDLK_F6:
        binding.key(Apsis::Key::F6);
        break;
      case SDLK_F7:
        binding.key(Apsis::Key::F7);
        break;
      case SDLK_F8:
        binding.key(Apsis::Key::F8);
        break;
      case SDLK_F9:
        binding.key(Apsis::Key::F9);
        break;
      case SDLK_F10:
        binding.key(Apsis::Key::F10);
        break;
      case SDLK_F11:
        binding.key(Apsis::Key::F11);
        break;
      case SDLK_F12:
        binding.key(Apsis::Key::F12);
        break;
      case SDLK_PERIOD:
        binding.key(Apsis::Key::PERIOD);
        break;
      case SDLK_COMMA:
        binding.key(Apsis::Key::COMMA);
        break;
      case SDLK_MINUS:
        binding.key(Apsis::Key::MINUS);
        break;
      case SDLK_EQUALS:
        binding.key(Apsis::Key::EQUALS);
        break;
      case SDLK_PLUS:
        binding.key(Apsis::Key::PLUS);
        break;
      case SDLK_LCTRL:
        binding.key(Apsis::Key::LEFTCONTROL);
        break;
      case SDLK_RCTRL:
        binding.key(Apsis::Key::RIGHTCONTROL);
        break;
      case SDLK_LALT:
        binding.key(Apsis::Key::LEFTALT);
        break;
      case SDLK_RALT:
        binding.key(Apsis::Key::RIGHTALT);
        break;
    }
  }
  else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
    binding.key((Apsis::Key::Code)(Apsis::Key::MOUSE_0 + event.button.button - 1));
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
  SDL_GL_SwapBuffers();
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
    if (_translateSDLKey(binding, sdl_event, pressed)) {
      event.binding(binding, pressed);
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

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

  //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,        1);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,        8);

  SDL_Surface* surf_display;
  if((surf_display = SDL_SetVideoMode(_video.resolutionX, _video.resolutionY, 32, SDL_HWSURFACE | SDL_OPENGL)) == NULL) {
    fprintf(stderr, "Unable to initialize SDL: SDL_SetVideoMode failed: %s\n", SDL_GetError());
    return false;
  }
  
  if (SDL_InitSubSystem(SDL_INIT_TIMER) < 0) {
    // Error
    char error[1024];
    sprintf(error, "Unable to initialize SDL: SDL_InitSubSystem(SDL_INIT_TIMER) failed: %s\n", SDL_GetError());
    throw error;
  }

  SDL_WM_SetCaption("Apsis", "Apsis");

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  return true;
#endif
}
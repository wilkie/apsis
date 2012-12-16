#include "iso-tasty/engine.h"

#include "math.h"

namespace Event {
  enum {
    MOVE_LEFT = 1,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
  };
}

IsoTasty::Engine::Engine(VideoSettings* video) {
  _input = new InputEngine();

  // Adding some events just because
  KeyBinding binding = {IsoTasty::Key::NONE};
  KeyBinding binding2 = {IsoTasty::Key::NONE};

  binding.key = IsoTasty::Key::LEFT;
  binding2.key = IsoTasty::Key::JOY_POV_LEFT;
  _input->keyBindings()->registerEvent("Move left",  Event::MOVE_LEFT, &binding, &binding2);
  binding.key = IsoTasty::Key::RIGHT;
  binding2.key = IsoTasty::Key::JOY_POV_RIGHT;
  _input->keyBindings()->registerEvent("Move right", Event::MOVE_RIGHT, &binding, &binding2);
  binding.key = IsoTasty::Key::UP;
  binding2.key = IsoTasty::Key::JOY_POV_UP;
  _input->keyBindings()->registerEvent("Move up",    Event::MOVE_UP, &binding, &binding2);
  binding.key = IsoTasty::Key::DOWN;
  binding2.key = IsoTasty::Key::JOY_POV_DOWN;
  _input->keyBindings()->registerEvent("Move down",  Event::MOVE_DOWN, &binding, &binding2);

  _video = *video;
  if (!_initialize()) {
    return;
  }

  _view = new Viewport(video->resolutionX, video->resolutionY);
  _renderer = new Renderer();
  newMap(50, 50);
}

void IsoTasty::Engine::newMap(unsigned int width, unsigned int height) {
  _map = new Map(width, height);
}

void IsoTasty::Engine::_draw() {
  _renderer->clear();
  _view->draw(_renderer, _map);
}

void IsoTasty::Engine::run() {
  int event;
  while(true) {
    event = _input->pollEvent();
    if (event == IsoTasty::InputEngine::QUIT_EVENT) {
      break;
    }
    else if (event) {
      _fireEvent(event);
    }
    else {
      _draw();
      SDL_GL_SwapBuffers();
      SDL_Delay(1);
    }
  }

  // destruct SDL
  SDL_Quit();
}

bool IsoTasty::Engine::_initialize() {
  if (_startSDL()) {
    _initViewport();
    return true;
  }

  fprintf(stderr, "Engine cannot initialize.");
  return false;
}

void IsoTasty::Engine::_initViewport() {
  _renderer->initializeViewport(_video.resolutionX, _video.resolutionY);
}

bool IsoTasty::Engine::_startSDL() {
#ifdef NO_SDL
  return false;
#else
  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,        1);

  SDL_Surface* surf_display;
  if((surf_display = SDL_SetVideoMode(_video.resolutionX, _video.resolutionY, 32, SDL_HWSURFACE | SDL_OPENGL)) == NULL) {
    fprintf(stderr, "Unable to initialize SDL: SDL_SetVideoMode failed\n");
    return false;
  }

  SDL_WM_SetCaption("IsoTasty", "IsoTasty");

  return true;
#endif
}

void IsoTasty::Engine::_fireEvent(int event) {
  if (_input->isEventHeld(event)) {
    switch (event) {
      case Event::MOVE_DOWN:
        _map->z(_map->z() + 1);
        break;
      case Event::MOVE_UP:
        _map->z(_map->z() - 1);
        break;
      case Event::MOVE_LEFT:
        _map->x(_map->x() - 1);
        break;
      case Event::MOVE_RIGHT:
        _map->x(_map->x() + 1);
        break;
    }
  }
  /*
  if (event->type == SDL_MOUSEBUTTONDOWN) {	
    switch(event->button.button) {
    case SDL_BUTTON_LEFT:
      break;
    }
  }
  else if (event->type == SDL_KEYDOWN) {
    switch(event->key.keysym.sym) {
    case SDLK_LEFT:
    case SDLK_h:
      if (event->key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT)) {
        _view->move(-0.5, 0.0);
      }	
      else {
        _map->x(_map->x() - 1);
      }
      break;
    case SDLK_RIGHT:
    case SDLK_l:
      if (event->key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT)) {
        _view->move(0.5, 0.0);
      }
      else {
        _map->x(_map->x() + 1);
      }
      break;
    case SDLK_UP:
    case SDLK_k:
      if (event->key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT)) {
        _view->move(0.0, -0.5);
      }
      else if (event->key.keysym.mod & (KMOD_RCTRL | KMOD_LCTRL)) {
        _map->raise(0.50);
      }
      else if (event->key.keysym.mod & (KMOD_RALT | KMOD_LALT)) {
        _map->lift(0.50);
      }
      else {
        _map->z(_map->z() - 1);
      }
      break;
    case SDLK_DOWN:
    case SDLK_j:
      if (event->key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT)) {
        _view->move(0.0, 0.5);
      }
      else if (event->key.keysym.mod & (KMOD_RCTRL | KMOD_LCTRL)) {
        _map->raise(-0.50);
      }
      else if (event->key.keysym.mod & (KMOD_RALT | KMOD_LALT)) {
        _map->lift(-0.50);
      }
      else {
        _map->z(_map->z() + 1);
      }
      break;
    case SDLK_COMMA:
      _view->rotate(15.0);
      break;
    case SDLK_PERIOD:
      _view->rotate(-15.0);
      break;
    case SDLK_EQUALS:
      _view->zoom(2);
      break;
    case SDLK_MINUS:
      _view->zoom(0.5);
      break;
    }
  }*/
}

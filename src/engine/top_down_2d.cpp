#include "apsis/engine/top_down_2d.h"
#include "apsis/sync/clock.h"

#include "apsis/input/key.h"

#include "math.h"

#include "apsis/primitives/camera.h"

#include "apsis/sync/clock.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

enum Event {
  INVALID,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,
  ZOOM_OUT,
  ZOOM_IN
};

 /*
  *  Construct an Engine initialized with the given display configuration
  */
Apsis::Engine::TopDown2d::TopDown2d(Apsis::Settings::Video& video) {
  _backend.initialize(video);
  _video = video;

  _x = video.resolutionX/2.0f;
  _z = video.resolutionY/2.0f;
  
  _zoom = 3.0f;

  _input = new Apsis::InputEngine();

  KeyBinding binding  = {Apsis::Key::NONE};
  KeyBinding binding2 = {Apsis::Key::NONE};

  binding.key = Apsis::Key::LEFT;
  binding2.key = Apsis::Key::JOY_POV_LEFT;
  _input->keyBindings()->registerEvent("Move left",  MOVE_LEFT, &binding, &binding2);
  binding.key = Apsis::Key::RIGHT;
  binding2.key = Apsis::Key::JOY_POV_RIGHT;
  _input->keyBindings()->registerEvent("Move right", MOVE_RIGHT, &binding, &binding2);
  binding.key = Apsis::Key::UP;
  binding2.key = Apsis::Key::JOY_POV_UP;
  _input->keyBindings()->registerEvent("Move up",    MOVE_UP, &binding, &binding2);
  binding.key = Apsis::Key::DOWN;
  binding2.key = Apsis::Key::JOY_POV_DOWN;
  _input->keyBindings()->registerEvent("Move down",  MOVE_DOWN, &binding, &binding2);
  binding.key = Apsis::Key::EQUALS;
  binding2.key = Apsis::Key::NONE;
  _input->keyBindings()->registerEvent("Zoom in",  ZOOM_IN, &binding, &binding2);
  binding.key = Apsis::Key::MINUS;
  binding2.key = Apsis::Key::NONE;
  _input->keyBindings()->registerEvent("Zoom out",  ZOOM_OUT, &binding, &binding2);

  Apsis::Primitives::SpriteSheet* sheet = new Apsis::Primitives::SpriteSheet("assets/graphics/floortilesbuffer.png");
  _map = new Apsis::World::Map(32, 30, sheet);

  _player1 = new Apsis::World::Actor("assets/actors/herr_von_speck.actor", 250, 175);
}

void Apsis::Engine::TopDown2d::run() {
  Apsis::Clock clock;

  int event;
  Apsis::Event core_event;

  while(true) {
    if (_backend.poll(core_event)) {
      event = _input->post(core_event);
      if (event == Apsis::InputEngine::QUIT_EVENT) {
        break;
      }
      else if (event) {
        _fireEvent(event);
      }
    }
    _update(clock.elapsedTime());
    _draw();
    _backend.swap();
  }

  _backend.quit();
}

void Apsis::Engine::TopDown2d::_draw() {
  bool orthographic = true;

  float rotation = 0.0;

  float aspect = (float)_video.resolutionX / (float)_video.resolutionY;

  float nearf = 1;
  float farf = 20.0;

  float fov = 45.0f;

  float top = tanf(fov * 0.5f) * nearf;
  float bottom = -top;

  float left = aspect * bottom;
  float right = aspect * top;

  glm::mat4 projection;

  float half_height = _video.resolutionY/2.0f;
  float half_width  = _video.resolutionX/2.0f;

  if (orthographic) {
    projection = glm::ortho(-half_width, half_width, -half_height, half_height);
  }
  else {
    projection = glm::perspective(fov, aspect, nearf, farf);
  }
  
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  _map->draw(projection,
             Primitives::Camera(glm::vec2((float)(int)(_x+0.5), (float)(int)(_z+0.5)), _zoom),
             glm::mat4(1.0));

  _player1->draw(projection,
                 Primitives::Camera(glm::vec2((float)(int)(_x+0.5), (float)(int)(_z+0.5)), _zoom),
                 glm::mat4(1.0));
}

void Apsis::Engine::TopDown2d::_update(float elapsed) {
  if (_input->isEventHeld(MOVE_DOWN)) {
    _z += 32*16*elapsed;
  }
  
  if (_input->isEventHeld(MOVE_UP)) {
    _z -= 32*16*elapsed;
  }

  if (_input->isEventHeld(MOVE_LEFT)) {
    _x -= 32*16*elapsed;
  }
  
  if (_input->isEventHeld(MOVE_RIGHT)) {
    _x += 32*16*elapsed;
  }
  
  if (_input->isEventHeld(ZOOM_OUT)) {
    _zoom -= 1.0f * elapsed;
    if (_zoom < 1.0f) {
      _zoom = 1.0f;
    }
  }
  
  if (_input->isEventHeld(ZOOM_IN)) {
    _zoom += 1.0f * elapsed;
  }

  if (_x > ((_map->width() * 32.0f - _video.resolutionX/2.0f/_zoom))) {
    _x = ((_map->width() * 32.0f - _video.resolutionX/2.0f/_zoom));
  }

  if (_x < (_video.resolutionX/2.0f/_zoom)) {
    _x = (_video.resolutionX/2.0f/_zoom);
  }

  if (_z > ((_map->height() * 32.0f - _video.resolutionY/2.0f/_zoom))) {
    _z = ((_map->height() * 32.0f - _video.resolutionY/2.0f/_zoom));
  }

  if (_z < (_video.resolutionY/2.0f/_zoom)) {
    _z = (_video.resolutionY/2.0f/_zoom);
  }

  _player1->update(elapsed);
}

void Apsis::Engine::TopDown2d::_fireEvent(int event) {
}

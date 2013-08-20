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

#include "apsis/agent/impeders/map_collider.h"
#include "apsis/agent/movers/linear.h"
#include "apsis/agent/movers/down.h"
#include "apsis/agent/movers/gridlock_down.h"
#include "apsis/agent/movers/up.h"
#include "apsis/agent/movers/gridlock_up.h"
#include "apsis/agent/movers/left.h"
#include "apsis/agent/movers/gridlock_left.h"
#include "apsis/agent/movers/right.h"
#include "apsis/agent/movers/gridlock_right.h"
#include "apsis/agent/movers/jump.h"
#include "apsis/agent/movers/wall_jump.h"
#include "apsis/agent/movers/fall.h"
#include "apsis/agent/movers/wall_slide.h"
#include "apsis/agent/movers/and.h"
#include "apsis/agent/movers/or.h"
#include "apsis/agent/movers/wiggler.h"

#include "apsis/registry/action.h"

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

  _zoom = 1.0f;

  _input = new Apsis::InputEngine();

  KeyBinding binding  = {Apsis::Key::NONE};
  KeyBinding binding2 = {Apsis::Key::NONE};

  binding.key = Apsis::Key::LEFT;
  binding2.key = Apsis::Key::JOY_POV_LEFT;
  _input->keyBindings()->registerEvent("Move left",  Apsis::Registry::Action::id("left"), &binding, &binding2);
  binding.key = Apsis::Key::RIGHT;
  binding2.key = Apsis::Key::JOY_POV_RIGHT;
  _input->keyBindings()->registerEvent("Move right", Apsis::Registry::Action::id("right"), &binding, &binding2);
  binding.key = Apsis::Key::UP;
  binding2.key = Apsis::Key::JOY_POV_UP;
  _input->keyBindings()->registerEvent("Move up",    Apsis::Registry::Action::id("up"), &binding, &binding2);
  binding.key = Apsis::Key::DOWN;
  binding2.key = Apsis::Key::JOY_POV_DOWN;
  _input->keyBindings()->registerEvent("Move down",  Apsis::Registry::Action::id("down"), &binding, &binding2);
  binding.key = Apsis::Key::Z;
  binding2.key = Apsis::Key::JOY_0;
  _input->keyBindings()->registerEvent("Jump",  Apsis::Registry::Action::id("jump"), &binding, &binding2);
  binding.key = Apsis::Key::EQUALS;
  binding2.key = Apsis::Key::NONE;
  _input->keyBindings()->registerEvent("Zoom in",  ZOOM_IN, &binding, &binding2);
  binding.key = Apsis::Key::MINUS;
  binding2.key = Apsis::Key::NONE;
  _input->keyBindings()->registerEvent("Zoom out",  ZOOM_OUT, &binding, &binding2);

  _map = new Apsis::World::Map("assets/maps/sample.json");
  _player1 = new Apsis::World::Actor(Apsis::Sprite::Thing::load("assets/actors/pink_spaceblob.json"), 300, 300);

  Apsis::Sprite::Sheet* hud = new Apsis::Sprite::Sheet("assets/graphics/hud_spritesheet.png");

  _health = new Apsis::Hud::FillerBar(hud, 15, 3, 6, 10, 10);
  _health->value(5);
  _numbers = new Apsis::Hud::Numbers(hud, 0, 10, _health->position().height + 20);
  _numbers->value(107243);

  Apsis::Primitives::Texture* texture = new Apsis::Primitives::Texture("assets/backgrounds/sky.png");

  _bg = new Apsis::World::Background(texture);

  _ball = new Apsis::World::Actor(Apsis::Sprite::Thing::load("assets/actors/coin.json"), 300, 300);

  // Bouncers should be a Responder agent (rule that applies when there is a collision!)

  // Coins wiggle
  _ball->attachMover(new Apsis::Agent::Movers::Wiggler(15.0f, 0.5f, 0.2f));

  // Ball hits walls
  _ball->attachImpeder(new Apsis::Agent::Impeders::MapCollider(_map));

  // Player cannot collide with map
  _player1->attachImpeder(new Apsis::Agent::Impeders::MapCollider(_map));

  // Player can move up
  //_player1->attachMover(new Apsis::Agent::Movers::Up(*_input, 256.0f));

  // Player can move down
  //_player1->attachMover(new Apsis::Agent::Movers::Down(*_input, 256.0f));

  // Player can move left
  _player1->attachMover(new Apsis::Agent::Movers::Left(*_input, 256.0f));

  // Player can move right
  _player1->attachMover(new Apsis::Agent::Movers::Right(*_input, 256.0f));

  // Player can jump
  // Player can wall jump
  _player1->attachMover(new Apsis::Agent::Movers::Jump(*_input, 220.0f, 512.0f, 2048.0f, 4096.0f, 496.0f));
  _player1->attachMover(new Apsis::Agent::Movers::WallJump(*_input, 220.0f, 512.0f, 1024.0f, 0.0f, 512.0f, 1024.0f));

  // Player can fall
  // Player can wall slide
  _player1->attachMover(new Apsis::Agent::Movers::Fall(0.0f, 1024.0f, 512.0f));
  _player1->attachMover(new Apsis::Agent::Movers::WallSlide(0.0, 1024.0f, 128.0f));
  //*/
}

void Apsis::Engine::TopDown2d::run() {
  Apsis::Clock clock;

  Apsis::Event core_event;

  while(true) {
    if (_backend.poll(core_event)) {
      int action;
      if (_input->post(core_event, action)) {
        if (action == Apsis::InputEngine::QUIT_EVENT) {
          break;
        }
        else if (action) {
          _fireEvent(action);
        }
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

  Apsis::Primitives::Camera camera = Primitives::Camera(glm::vec2((float)(int)(_x+0.5), (float)(int)(_z+0.5)), _zoom);

  
  _bg->draw(projection, camera,
            glm::mat4(1.0));

  _map->draw(projection,
             camera,
             glm::mat4(1.0));
  
  _ball->draw(projection, camera);
  _player1->draw(projection, camera);
  
  // HUD camera
  Apsis::Primitives::Camera hud_camera = Primitives::Camera(glm::vec2(half_width, half_height), 1.0);
  
  _numbers->draw(projection,
                 hud_camera);
  _health->draw(projection,
                hud_camera);
}

void Apsis::Engine::TopDown2d::_update(float elapsed) {
  if (_input->isEventHeld(ZOOM_OUT)) {
    _zoom -= 1.0f * elapsed;
    if (_zoom < 1.0f) {
      _zoom = 1.0f;
    }
  }

  if (_input->isEventHeld(ZOOM_IN)) {
    _zoom += 1.0f * elapsed;
  }

  _x = _player1->position().x;
  _z = _player1->position().y;

  if (_x > ((_map->width() * _map->tileWidth() - _video.resolutionX/2.0f/_zoom))) {
    _x = ((_map->width() * _map->tileWidth() - _video.resolutionX/2.0f/_zoom));
  }

  if (_x < (_video.resolutionX/2.0f/_zoom)) {
    _x = (_video.resolutionX/2.0f/_zoom);
  }

  if (_z > ((_map->height() * _map->tileHeight() - _video.resolutionY/2.0f/_zoom))) {
    _z = ((_map->height() * _map->tileHeight() - _video.resolutionY/2.0f/_zoom));
  }

  if (_z < (_video.resolutionY/2.0f/_zoom)) {
    _z = (_video.resolutionY/2.0f/_zoom);
  }

  _ball->update(elapsed);
  _player1->update(elapsed);
}

void Apsis::Engine::TopDown2d::_fireEvent(int event) {
}

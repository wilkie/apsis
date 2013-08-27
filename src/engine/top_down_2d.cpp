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

#include "apsis/rules/map_collider.h"
#include "apsis/rules/actor_collider.h"

#include "apsis/registry/action.h"

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

 /*
  *  Construct an Engine initialized with the given display configuration
  */
Apsis::Engine::TopDown2d::TopDown2d(Apsis::Settings::Video& video) {
  _backend.initialize(video);
  _video = video;

  _x = video.resolutionX/2.0f;
  _z = video.resolutionY/2.0f;

  _zoom = 1.0f;

  _input = &Apsis::Engine::Input::default();

  _map = &Apsis::World::Map::load("assets/maps/sample.json");

  Apsis::Sprite::Sheet* hud = new Apsis::Sprite::Sheet("assets/graphics/hud_spritesheet.png");

  _health = new Apsis::Hud::FillerBar(hud, 15, 3, 6, 10, 10);
  _health->value(5);
  _numbers = new Apsis::Hud::Numbers(hud, 0, 10, _health->position().height + 20);
  _numbers->value(107243);

  Apsis::Primitives::Texture* texture = new Apsis::Primitives::Texture("assets/backgrounds/sky.png");

  _ball = _scene.addActor(Apsis::Sprite::Thing::load("assets/actors/coin.json"), 600, 300);
  _player = _scene.addActor(Apsis::Sprite::Thing::load("assets/actors/pink_spaceblob.json"), 300, 300);
  _scene.addMap(*_map);

  Apsis::Registry::Action::load("assets/bindings/input.json");
}

void Apsis::Engine::TopDown2d::run() {
  Apsis::Clock clock;

  Apsis::Engine::Event core_event;

  unsigned int action_id = 0;
  while(true) {
    if (_backend.poll(core_event)) {
      if (core_event.type() == Apsis::Engine::Event::Type::Press) {
        if (_input->press(core_event.binding(), action_id)) {
          _scene.act(action_id, true);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::Release) {
        if (_input->release(core_event.binding(), action_id)) {
          _scene.act(action_id, false);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::SystemEvent) {
        if (core_event.systemEvent() == Apsis::Engine::Event::SystemEvent::Quit) {
          break;
        }
      }
    }
    _scene.update(clock.elapsedTime());

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

  _scene.draw(projection, camera, glm::mat4(1.0f));
  
  // HUD camera
  Apsis::Primitives::Camera hud_camera = Primitives::Camera(glm::vec2(half_width, half_height), 1.0);
  
  _numbers->draw(projection,
                 hud_camera);
  _health->draw(projection,
                hud_camera);
}

void Apsis::Engine::TopDown2d::_update(float elapsed) {
}

void Apsis::Engine::TopDown2d::_fireEvent(int event) {
}

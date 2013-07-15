#include "apsis/engine/top_down_2d.h"
#include "apsis/sync/clock.h"

#include "apsis/input/key.h"

#include "math.h"

#include "apsis/primitives/camera.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

 /*
  *  Construct an Engine initialized with the given display configuration
  */
Apsis::Engine::TopDown2d::TopDown2d(Apsis::Settings::Video& video) {
  _backend.initialize(video);

  _input = new Apsis::InputEngine();

  Apsis::Primitives::SpriteSheet* sheet = new Apsis::Primitives::SpriteSheet("assets/graphics/floortiles.png");
  _map = new Apsis::World::Map(32, 30, sheet);

  Apsis::World::Actor* player1 = new Apsis::World::Actor("assets/actors/herr_von_speck.actor", 250, 175);
}

void Apsis::Engine::TopDown2d::run() {
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
    _draw();
    _backend.swap();
  }

  _backend.quit();
}

void Apsis::Engine::TopDown2d::_draw() {
  bool orthographic = true;
  double rotation = 0.0;
  double translationX = 0.0;
  double translationZ = 0.0;
  double zoom = 1.0;
  zoom /= 2;

  float dist = sqrtf(1.0f / 3.0f);
  float aspect = (float)_video.resolutionX / (float)_video.resolutionY;

  float nearf = 1;
  float farf = 20.0;

  float fov = 45.0f;

  float top = tanf(fov * 0.5f) * nearf;
  float bottom = -top;

  float left = aspect * bottom;
  float right = aspect * top;

  glm::mat4 projection;

  if (orthographic) {
    projection = glm::ortho(left*4, right*4, bottom*4, top*4);
  }
  else {
    projection = glm::perspective(fov, aspect, nearf, farf);
  }

  glm::mat4 view = glm::rotate(
                     glm::translate(
                       glm::mat4(1.0f),
                       glm::vec3(0.0f, 0.0f, -5.0f)),
                     35.264f, glm::vec3(1.0f, 0.0f, 0.0f));

  if (!orthographic) {
    view = glm::rotate(view, 35.264f, glm::vec3(1.0f, 0.0f, 0.0f));
  } 

  view = glm::scale(
           glm::translate(
             glm::rotate(view, (float)rotation, glm::vec3(0.0f, 1.0f, 0.0f)),
             glm::vec3(translationX, 0, translationZ)),
           glm::vec3(zoom, zoom, zoom));

  glm::mat4 model = glm::mat4(1.0);
  
  _map->draw(projection, Primitives::Camera(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(1.0, 0.0, 0.0))), model);
}

void Apsis::Engine::TopDown2d::_update() {
}

void Apsis::Engine::TopDown2d::_fireEvent(int event) {
  if (_input->isEventHeld(event)) {
    switch (event) {
    case 0:
    default:
      break;
    }
  }
}

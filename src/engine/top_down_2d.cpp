#include "apsis/engine/top_down_2d.h"
#include "apsis/sync/clock.h"

#include "apsis/input/key.h"

#include "math.h"

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
  _renderer->clear();
  _map->draw(*_renderer);
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

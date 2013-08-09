#include "apsis/agent/movers/down.h"

#include "apsis/registry/action.h"

Apsis::Agent::Movers::Down::Down(Apsis::InputEngine& inputEngine,
	                             float               speed)
  : _speed(speed), Apsis::Agent::Mover("can move down") {
  _inputEngine = &inputEngine;

  _downAction = Apsis::Registry::Action::id("down");
}

bool Apsis::Agent::Movers::Down::update(float elapsed,
                                        Apsis::World::Object& object,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (_inputEngine->isEventHeld(_downAction)) {
    updated.y += elapsed * _speed;
    return true;
  }

  return false;
}
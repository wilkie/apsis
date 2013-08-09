#include "apsis/agent/movers/right.h"

#include "apsis/registry/action.h"

Apsis::Agent::Movers::Right::Right(Apsis::InputEngine& inputEngine,
                                   float speed)
  : _speed(speed),
    Apsis::Agent::Mover("can move right") {
  _inputEngine = &inputEngine;

  _rightAction = Apsis::Registry::Action::id("right");
}

bool Apsis::Agent::Movers::Right::update(float elapsed,
                                         Apsis::World::Object& object,
                                         const Apsis::Geometry::Rectangle& original,
                                         Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (_inputEngine->isEventHeld(_rightAction)) {
    updated.x += elapsed * _speed;
    return true;
  }

  return false;
}
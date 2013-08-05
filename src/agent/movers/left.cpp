#include "apsis/agent/movers/left.h"

#include "apsis/registry/action.h"

Apsis::Agent::Movers::Left::Left(Apsis::InputEngine& inputEngine, float speed)
  : _speed(speed),
    Apsis::Agent::Mover("can move left") {
  _inputEngine = &inputEngine;

  _leftAction = Apsis::Registry::Action::id("left");
}

bool Apsis::Agent::Movers::Left::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (_inputEngine->isEventHeld(_leftAction)) {
    updated.x -= elapsed * _speed;
    return true;
  }

  return false;
}
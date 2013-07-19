#include "apsis/agent/movers/right.h"

Apsis::Agent::Movers::Right::Right(Apsis::InputEngine& inputEngine,
                                   float speed)
  : _speed(speed) {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Right::update(float elapsed,
                                         std::set<unsigned int>& states,
                                         Apsis::Geometry::Rectangle& original,
                                         Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_RIGHT)) {
    updated.x += elapsed * 96.0f;
    return true;
  }

  return false;
}

char* Apsis::Agent::Movers::Right::rule() {
  return "can move right";
}
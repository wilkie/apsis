#include "apsis/agent/movers/right.h"

Apsis::Agent::Movers::Right::Right(Apsis::InputEngine& inputEngine,
                                   float speed)
  : _speed(speed),
    Apsis::Agent::Mover("can move right") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Right::update(float elapsed,
                                         std::set<unsigned int>& states,
                                         const Apsis::Geometry::Rectangle& original,
                                         Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_RIGHT)) {
    updated.x += elapsed * _speed;
    return true;
  }

  return false;
}
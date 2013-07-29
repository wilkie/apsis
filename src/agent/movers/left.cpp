#include "apsis/agent/movers/left.h"

Apsis::Agent::Movers::Left::Left(Apsis::InputEngine& inputEngine, float speed)
  : _speed(speed),
    Apsis::Agent::Mover("can move left") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Left::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_LEFT)) {
    updated.x -= elapsed * _speed;
    return true;
  }

  return false;
}
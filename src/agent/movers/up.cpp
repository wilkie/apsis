#include "apsis/agent/movers/up.h"

Apsis::Agent::Movers::Up::Up(Apsis::InputEngine& inputEngine,
	                         float               speed)
  : _speed(speed), Apsis::Agent::Mover("can move up") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Up::update(float elapsed,
                                      std::set<unsigned int>& states,
                                      const Apsis::Geometry::Rectangle& original,
                                      Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_UP)) {
    updated.y -= elapsed * _speed;
    return true;
  }

  return false;
}
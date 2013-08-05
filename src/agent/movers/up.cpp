#include "apsis/agent/movers/up.h"

#include "apsis/registry/action.h"

Apsis::Agent::Movers::Up::Up(Apsis::InputEngine& inputEngine,
	                         float               speed)
  : _speed(speed), Apsis::Agent::Mover("can move up") {
  _inputEngine = &inputEngine;

  _upAction = Apsis::Registry::Action::id("up");
}

bool Apsis::Agent::Movers::Up::update(float elapsed,
                                      std::set<unsigned int>& states,
                                      const Apsis::Geometry::Rectangle& original,
                                      Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (_inputEngine->isEventHeld(_upAction)) {
    updated.y -= elapsed * _speed;
    return true;
  }

  return false;
}
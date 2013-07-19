#include "apsis/agent/movers/fall.h"

Apsis::Agent::Movers::Fall::Fall(float startingVelocity,
                                 float acceleration,
                                 float terminalVelocity)
  : _startingVelocity(startingVelocity),
    _acceleration(acceleration),
    _terminalVelocity(terminalVelocity) {

  _velocity = _startingVelocity;
}

bool Apsis::Agent::Movers::Fall::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(Apsis::State::COLLIDE_DOWN_WITH_MAP) > 0) {
    _velocity = _startingVelocity;
  }

  if ((states.count(Apsis::State::JUMPING) == 0)) {
    states.erase(Apsis::State::COLLIDE_DOWN_WITH_MAP);

    _velocity += _acceleration * elapsed;
    if (_velocity > _terminalVelocity) {
      _velocity = _terminalVelocity;
    }
    
    updated.y += elapsed * _velocity;

    return true;
  }
  else {
    _velocity = _startingVelocity;
  }

  return false;
}

char* Apsis::Agent::Movers::Fall::rule() {
  return "can fall";
}
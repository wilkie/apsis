#include "apsis/agent/movers/fall.h"

#include "apsis/registry/state.h"

Apsis::Agent::Movers::Fall::Fall(float startingVelocity,
                                 float acceleration,
                                 float terminalVelocity)
  : _startingVelocity(startingVelocity),
    _acceleration(acceleration),
    _terminalVelocity(terminalVelocity),
    Apsis::Agent::Mover("fall") {

  _velocity = _startingVelocity;

  _collideWithTopState  = Apsis::Registry::State::id("collideWithTop");

  _jumpingState  = Apsis::Registry::State::id("jumping");
}

bool Apsis::Agent::Movers::Fall::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(_collideWithTopState) > 0) {
    _velocity = _startingVelocity;
  }

  if ((states.count(_jumpingState) == 0)) {
    states.erase(_collideWithTopState);

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
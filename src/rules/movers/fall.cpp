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
                                        Apsis::World::Object& object,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (object.isEnabled(_collideWithTopState)) {
    _velocity = _startingVelocity;
  }

  if (!object.isEnabled(_jumpingState)) {
    object.disableState(_collideWithTopState);

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
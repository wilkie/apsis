#include "apsis/agent/movers/wall_slide.h"

#include "apsis/registry/state.h"

Apsis::Agent::Movers::WallSlide::WallSlide(float startingVelocity,
                                           float acceleration,
                                           float terminalVelocity)
  : _startingVelocity(startingVelocity),
    _acceleration(acceleration),
    _terminalVelocity(terminalVelocity),
    Apsis::Agent::Mover("wall slide") {

  _velocity = _startingVelocity;

  _collideWithLeftState   = Apsis::Registry::State::id("collideWithLeft");
  _collideWithRightState  = Apsis::Registry::State::id("collideWithRight");

  _jumpingState  = Apsis::Registry::State::id("jumping");

  supercede("fall");
}

bool Apsis::Agent::Movers::WallSlide::update(float elapsed,
                                             Apsis::World::Object& object,
                                             const Apsis::Geometry::Rectangle& original,
                                             Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (!object.isEnabled(_collideWithLeftState) &&
      !object.isEnabled(_collideWithRightState)) {
    _velocity = _startingVelocity;
  }

  if ((!object.isEnabled(_jumpingState)) &&
      ((object.isEnabled(_collideWithLeftState)) ||
       (object.isEnabled(_collideWithRightState)))) {
    object.disableState(_collideWithLeftState);
    object.disableState(_collideWithRightState);

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
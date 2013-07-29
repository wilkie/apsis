#include "apsis/agent/movers/wall_slide.h"

Apsis::Agent::Movers::WallSlide::WallSlide(float startingVelocity,
                                           float acceleration,
                                           float terminalVelocity)
  : _startingVelocity(startingVelocity),
    _acceleration(acceleration),
    _terminalVelocity(terminalVelocity),
    Apsis::Agent::Mover("wall slide") {

  _velocity = _startingVelocity;

  supercede("fall");
}

bool Apsis::Agent::Movers::WallSlide::update(float elapsed,
                                             std::set<unsigned int>& states,
                                             const Apsis::Geometry::Rectangle& original,
                                             Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(Apsis::State::COLLIDE_LEFT_WITH_MAP) == 0 &&
      states.count(Apsis::State::COLLIDE_RIGHT_WITH_MAP) == 0) {
    _velocity = _startingVelocity;
  }

  if ((states.count(Apsis::State::JUMPING) == 0) &&
      ((states.count(Apsis::State::COLLIDE_LEFT_WITH_MAP) > 0) ||
       (states.count(Apsis::State::COLLIDE_RIGHT_WITH_MAP) > 0))) {
    states.erase(Apsis::State::COLLIDE_LEFT_WITH_MAP);
    states.erase(Apsis::State::COLLIDE_RIGHT_WITH_MAP);

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
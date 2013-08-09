#include "apsis/agent/movers/linear.h"

Apsis::Agent::Movers::Linear::Linear(float direction, float speed)
  : _speed(speed),
    _direction(direction),
    Apsis::Agent::Mover("moves in a line") {
}

bool Apsis::Agent::Movers::Linear::update(float elapsed,
                                          Apsis::World::Object& object,
                                          const Apsis::Geometry::Rectangle& original,
                                          Apsis::Geometry::Point& updated) {
  updated = original.center();

  updated.x += cos(_direction) * (elapsed * _speed);
  updated.y += sin(_direction) * (elapsed * _speed);
  return true;
}
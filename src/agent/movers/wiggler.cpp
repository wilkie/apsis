#include "apsis/agent/movers/wiggler.h"

#include "apsis/registry/state.h"

Apsis::Agent::Movers::Wiggler::Wiggler(float amount,
                                       float time,
                                       float stall)
  : _amount(amount),
    _time(time),
    _stall(stall),
    Apsis::Agent::Mover("wiggle") {

  _direction = 0.0f;

  _travelled = 0.0f;
}

bool Apsis::Agent::Movers::Wiggler::update(float elapsed,
                                           Apsis::World::Object& object,
                                           const Apsis::Geometry::Rectangle& original,
                                           Apsis::Geometry::Point& updated) {

  float travel = elapsed * (_amount / _time);
  _travelled += travel;

  if (_travelled > _amount) {
    travel = _amount - (_travelled - elapsed);
    _travelled = 0;
    _direction = _direction - 3.1415296;
  }

  updated.x = original.x + sin(_direction) * travel;
  updated.y = original.y + cos(_direction) * travel;
  return true;
}
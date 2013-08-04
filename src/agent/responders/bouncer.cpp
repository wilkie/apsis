#include <apsis/agent/responders/bouncer.h>

Apsis::Agent::Responders::Bouncer::Bouncer(float direction, float speed)
  : _speed(speed),
    _direction(direction),
    Apsis::Agent::Responder("bounces") {
}

bool Apsis::Agent::Responders::Bouncer::uponEvent(std::set<unsigned int>& states,
                                                  Apsis::Geometry::Rectangle& original,
                                                  Apsis::Geometry::Point& intended) {
  return true;
}
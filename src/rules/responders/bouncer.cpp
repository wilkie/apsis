#include <apsis/agent/responders/bouncer.h>

Apsis::Agent::Responders::Bouncer::Bouncer(float direction, float speed)
  : _speed(speed),
    _direction(direction),
    Apsis::Agent::Responder("bounces") {
}

// Once it sees a collision event, it uses that to change to the opposite direction.
// Events will be specified by id which will be registered in the initialization code.
// Need a key/value store to be passed along for modification with key registry in
//   initialization.
bool Apsis::Agent::Responders::Bouncer::uponEvent(Apsis::World::Object& object,
                                                  Apsis::Geometry::Rectangle& original,
                                                  Apsis::Geometry::Point& intended) {
  return true;
}
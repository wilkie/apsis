#include "apsis/agent/impeders/actor_collider.h"

Apsis::Agent::Impeders::ActorCollider::ActorCollider(std::vector<Apsis::World::Actor>& list) {
  _actors = &list;
}

bool Apsis::Agent::Impeders::ActorCollider::update(Apsis::Geometry::Point& original, Apsis::Geometry::Point& intended) {
  // Collide with actors
  return false;
}
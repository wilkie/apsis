#include "apsis/agent/impeder.h"

#include "apsis/geometry/point.h"

Apsis::Agent::Impeder::Impeder() {
}

bool Apsis::Agent::Impeder::update(Apsis::World::Object& object,
                                   Apsis::Geometry::Rectangle& original,
                                   Apsis::Geometry::Point& intended) {
  return false;
}

const char* Apsis::Agent::Impeder::rule() {
  return "Unknown impeder rule";
}

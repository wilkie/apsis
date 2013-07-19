#include "apsis/agent/mover.h"

#include "apsis/geometry/point.h"

Apsis::Agent::Mover::Mover() {
}

bool Apsis::Agent::Mover::update(float elapsed,
                                 std::set<unsigned int>& states,
                                 Apsis::Geometry::Rectangle& original,
                                 Apsis::Geometry::Point& updated) {
  return false;
}

char* Apsis::Agent::Mover::rule() {
  return "Unknown move rule";
}
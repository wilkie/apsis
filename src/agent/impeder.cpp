#include "apsis/agent/impeder.h"

#include "apsis/geometry/point.h"

Apsis::Agent::Impeder::Impeder() {
}

bool Apsis::Agent::Impeder::update(std::set<unsigned int>& states,
                                   Apsis::Geometry::Rectangle& original,
                                   Apsis::Geometry::Point& intended) {
  return false;
}

char* Apsis::Agent::Impeder::rule() {
  return "Unknown impeder rule";
}
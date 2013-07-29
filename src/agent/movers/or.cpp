#include "apsis/agent/movers/or.h"

Apsis::Agent::Movers::Or::Or(Apsis::Agent::Mover* first,
                             Apsis::Agent::Mover* second)
  : Apsis::Agent::Mover("or") {
  _movers.push_back(first);
  _movers.push_back(second);
}

bool Apsis::Agent::Movers::Or::update(float elapsed,
                                      std::set<unsigned int>& states,
                                      const Apsis::Geometry::Rectangle& original,
                                      Apsis::Geometry::Point& updated) {
  bool moved = false;

  for (unsigned int i = 0; i < _movers.size(); i++) {
    if (_movers[i]->update(elapsed, states, original, updated)) {
      return true;
    }
  }

  return false;
}
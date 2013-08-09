#include <apsis/agent/movers/and.h>

Apsis::Agent::Movers::And::And(Apsis::Agent::Mover* first,
                               Apsis::Agent::Mover* second)
  : Apsis::Agent::Mover("and") {
  _movers.push_back(first);
  _movers.push_back(second);
}

bool Apsis::Agent::Movers::And::update(float elapsed,
                                       Apsis::World::Object& object,
                                       const Apsis::Geometry::Rectangle& original,
                                       Apsis::Geometry::Point& updated) {
  bool moved = false;

  Apsis::Geometry::Rectangle positioned = original;

  for (unsigned int i = 0; i < _movers.size(); i++) {
    if (_movers[i]->update(elapsed, object, positioned, updated)) {
      positioned.x = updated.x;
      positioned.y = updated.y;
      moved = true;
    }
  }

  updated = positioned.center();

  return moved;
}
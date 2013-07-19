#ifndef APSIS_AGENT_MOVER_H
#define APSIS_AGENT_MOVER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <set>

namespace Apsis {
  namespace Agent {
    class Mover {
    public:
      Mover();

      /*
       *  Update intended point and return reason for change.
       */
      virtual bool update(float elapsed,
                          std::set<unsigned int>& states,
                          Apsis::Geometry::Rectangle& original,
                          Apsis::Geometry::Point& updated);

      virtual char* rule();
    };
  }
}

#endif

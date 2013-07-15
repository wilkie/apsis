#ifndef APSIS_AGENT_MOVER_H
#define APSIS_AGENT_MOVER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    class Mover {
    public:
      Mover();

      /*
       *  Update intended point and return reason for change.
       */
      virtual bool update(Apsis::Geometry::Rectangle& original,
                          Apsis::Geometry::Point& updated);
    };
  }
}

#endif

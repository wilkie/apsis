#ifndef APSIS_AGENT_IMPEDER_H
#define APSIS_AGENT_IMPEDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <set>

namespace Apsis {
  namespace Agent {
    class Impeder {
    public:
      Impeder();

      /*
       *  Update intended point and return reason for change.
       */
      virtual bool update(std::set<unsigned int>& states,
                          Apsis::Geometry::Rectangle& original,
                          Apsis::Geometry::Point& intended);
      virtual const char* rule();
    };
  }
}

#endif

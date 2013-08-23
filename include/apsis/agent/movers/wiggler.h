#ifndef APSIS_AGENT_MOVERS_WIGGLER_H
#define APSIS_AGENT_MOVERS_WIGGLER_H

#include "apsis/agent/mover.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Wiggler {
      public:
        static bool update(float elapsed,
                           const Apsis::World::Scene& scene,
                           const unsigned int objectId,
                           Apsis::World::Object& object);
      };
    }
  }
}

#endif
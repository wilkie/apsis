#ifndef APSIS_RULES_FALL_H
#define APSIS_RULES_FALL_H

#include "apsis/world/scene.h"
#include "apsis/world/object.h"

namespace Apsis {
  namespace Rules {
    class Fall {
    public:
      static bool update(float elapsed,
                         const Apsis::World::Scene& scene,
                         Apsis::World::Object& object);
    };
  }
}

#endif

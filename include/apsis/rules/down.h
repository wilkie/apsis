#ifndef APSIS_RULES_DOWN_H
#define APSIS_RULES_DOWN_H

#include "apsis/world/scene.h"
#include "apsis/world/object.h"

#include "apsis/registry/action.h"

namespace Apsis {
  namespace Rules {
    class Down {
    public:
      static bool update(float elapsed,
                          const Apsis::World::Scene& scene,
                          Apsis::World::Object& object);

      static const char* action;

      static bool act(unsigned int action_id,
                      bool held,
                      const Apsis::World::Scene& scene,
                      Apsis::World::Object& object);
    };
  }
}

#endif

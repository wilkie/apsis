#ifndef APSIS_AGENT_MOVERS_RIGHT_H
#define APSIS_AGENT_MOVERS_RIGHT_H

#include "apsis/world/scene.h"
#include "apsis/world/object.h"

#include "apsis/registry/action.h"

namespace Apsis {
  namespace Agent {
    namespace Rules {
      class Right {
      public:
        static bool update(float elapsed,
                           const Apsis::World::Scene& scene,
                           const unsigned int objectId,
                           Apsis::World::Object& object);

        static std::string action;

        static bool act(unsigned int action_id,
                        bool held,
                        const Apsis::World::Scene& scene,
                        const unsigned int objectId,
                        Apsis::World::Object& object);
      };
    }
  }
}

#endif

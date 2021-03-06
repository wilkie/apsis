#ifndef APSIS_RULES_ADD_VALUE_H
#define APSIS_RULES_ADD_VALUE_H

#include "apsis/world/scene.h"
#include "apsis/world/object.h"

#include "apsis/registry/event.h"

namespace Apsis {
  namespace Rules {
    class AddValue {
    public:
      static void respond(unsigned int event_id,
                          const Apsis::World::Scene& scene,
                          Apsis::World::Object& object);
    };
  }
}

#endif
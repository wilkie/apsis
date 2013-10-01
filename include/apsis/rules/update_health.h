#ifndef APSIS_RULES_UPDATE_HEALTH_H
#define APSIS_RULES_UPDATE_HEALTH_H

#include "apsis/world/scene.h"
#include "apsis/world/object.h"

#include "apsis/registry/event.h"

namespace Apsis {
  namespace Rules {
    class UpdateHealth {
    public:
      static void respond(unsigned int event_id,
                          const Apsis::World::Scene&,
                          Apsis::World::Object& object);
    };
  }
}

#endif
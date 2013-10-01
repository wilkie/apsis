#ifndef APSIS_RULES_RESPONSE_FUNCTION_H
#define APSIS_RULES_RESPONSE_FUNCTION_H

#include <apsis/world/object.h>

namespace Apsis {
  namespace World {
    class Scene;
  }

  namespace Rules {
    // Respond to events
    // TODO: document this
    typedef void (*ResponseFunction)(unsigned int event_id,
                                     const Apsis::World::Scene&,
                                     Apsis::World::Object& object);
  }
}

#endif

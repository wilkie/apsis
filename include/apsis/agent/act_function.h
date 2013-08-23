#ifndef APSIS_AGENT_ACT_FUNCTION_H
#define APSIS_AGENT_ACT_FUNCTION_H

#include <apsis/world/object.h>

namespace Apsis {
  namespace World {
    class Scene;
  }

  namespace Agent {
    // Act
    // TODO: document this
    typedef bool (*ActFunction)(unsigned int action_id,
                                bool held,
                                const Apsis::World::Scene&,
                                const unsigned int objectId,
                                Apsis::World::Object& object);
  }
}

#endif

#ifndef APSIS_RULES_ACT_FUNCTION_H
#define APSIS_RULES_ACT_FUNCTION_H

#include <apsis/world/object.h>

namespace Apsis {
  namespace World {
    class Scene;
  }

  namespace Rules {
    // Act
    // TODO: document this
    typedef bool (*ActFunction)(unsigned int action_id,
                                bool held,
                                const Apsis::World::Scene&,
                                Apsis::World::Object& object);
  }
}

#endif

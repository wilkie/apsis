#ifndef APSIS_AGENT_MOVER_H
#define APSIS_AGENT_MOVER_H

#include <apsis/world/object.h>

namespace Apsis {
  namespace World {
    class Scene;
  }

  namespace Agent {
    // TODO: World::Object should have a transactional update so
    //       that each update function sees the same object.
    typedef bool (*UpdateFunction)(float elapsed,
                                   const Apsis::World::Scene&,
                                   const unsigned int objectId,
                                   Apsis::World::Object& object);
  }
}

#endif

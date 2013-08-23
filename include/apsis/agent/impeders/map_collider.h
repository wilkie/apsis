#ifndef APSIS_AGENT_IMPEDERS_MAP_COLLIDER_H
#define APSIS_AGENT_IMPEDERS_MAP_COLLIDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include "apsis/world/scene.h"
#include "apsis/agent/impeder.h"

#include <set>

namespace Apsis {
  namespace World {
    class Actor;
  }

  namespace Agent {
    namespace Impeders {
      class MapCollider {
      public:
        static bool collide(const Apsis::World::Scene& scene,
                            const unsigned int objectId,
                            const Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            const Apsis::Geometry::Point& intended,
                            unsigned int collidedWith,
                            Apsis::Geometry::Point& clipped);
      };
    }
  }
}

#endif

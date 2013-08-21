#ifndef APSIS_AGENT_IMPEDER_H
#define APSIS_AGENT_IMPEDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include "apsis/world/object.h"

#include <set>

namespace Apsis {
  namespace World {
    class Scene;
  }

  namespace Agent {
    /*
     *  A CollideFunction takes five arguments that represent the scene, the
     *  object in movement, and the point where the object will end up. The
     *  role of the function is to set the next two arguments. It should, if
     *  there is a collision, set collidedWith with the object id of the object
     *  that was involved in the collision, and it should also set clipped with
     *  the point the object in movement will actually stop. It will return
     *  true if there was a collision and false otherwise. When false is
     *  returned, the parameters collidedWith and clipped should be ignored.
     */
    typedef bool (*CollideFunction)(const Apsis::World::Scene&,
                                    const unsigned int objectId,
                                    const Apsis::World::Object& object,
                                    const Apsis::Geometry::Rectangle& original,
                                    const Apsis::Geometry::Point& intended,
                                    unsigned int collidedWith,
                                    Apsis::Geometry::Point& clipped);
  }
}

#endif

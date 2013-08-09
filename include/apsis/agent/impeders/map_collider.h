#ifndef APSIS_AGENT_IMPEDERS_MAP_COLLIDER_H
#define APSIS_AGENT_IMPEDERS_MAP_COLLIDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include "apsis/agent/impeder.h"

#include <set>

namespace Apsis {
  namespace World {
    class Actor;
  }

  namespace Agent {
    namespace Impeders {
      class MapCollider : public Apsis::Agent::Impeder {
      public:
        MapCollider(Apsis::World::Map* map);

        /*
          *  Update intended point and return reason for change.
          */
        virtual bool update(Apsis::World::Object& object,
                            Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& intended);
        virtual const char* rule();
      private:
        bool _canFall(Apsis::Geometry::Rectangle& from);
        bool _canJump(Apsis::Geometry::Rectangle& from);
        bool _againstWall(Apsis::Geometry::Rectangle& from, bool checkLeft);

        Apsis::World::Map* _map;

        unsigned int _collideWithLeftState;
        unsigned int _collideWithRightState;
        unsigned int _collideWithTopState;
        unsigned int _collideWithBottomState;
      };
    }
  }
}

#endif

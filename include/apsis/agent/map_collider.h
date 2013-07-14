#ifndef APSIS_AGENT_MAP_COLLIDER_H
#define APSIS_AGENT_MAP_COLLIDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    class MapCollider {
      public:
        MapCollider(Apsis::World::Map* map);

        /*
          *  Determines how far along the given vector the given Rectangle can
          *  move. Returns true when the Rectangle can move the entire distance. 
          */
        bool move(Apsis::Geometry::Rectangle* p,
                  Apsis::Geometry::Point* v);

      private:
        Apsis::World::Map* _map;
    };
  }
}

#endif

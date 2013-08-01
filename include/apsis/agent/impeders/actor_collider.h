#ifndef APSIS_AGENT_IMPEDERS_ACTOR_COLLIDER_H
#define APSIS_AGENT_IMPEDERS_ACTOR_COLLIDER_H

#include "apsis/world/actor.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include "apsis/agent/impeder.h"

#include <vector>
#include <set>

namespace Apsis {
  namespace World {
    class Actor;
  }

  namespace Agent {
    namespace Impeders {
      class ActorCollider : public Apsis::Agent::Impeder {
      public:
        ActorCollider(std::vector<Apsis::World::Actor>& list);
        virtual bool update(std::set<unsigned int>& states,
                            Apsis::Geometry::Point& original,
                            Apsis::Geometry::Point& intended);
        virtual const char* rule();
      private:
        std::vector<Apsis::World::Actor>* _actors;
      };
    }
  }
}

#endif

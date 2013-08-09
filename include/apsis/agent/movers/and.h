#ifndef APSIS_AGENT_MOVERS_AND_H
#define APSIS_AGENT_MOVERS_AND_H

#include <apsis/agent/mover.h>
#include <apsis/input/input_engine.h>

#include <apsis/geometry/rectangle.h>
#include <apsis/geometry/point.h>

#include <apsis/world/object.h>

#include <vector>

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class And : public Apsis::Agent::Mover {
      public:
        And(Apsis::Agent::Mover* first,
            Apsis::Agent::Mover* second);

        virtual bool update(float elapsed,
                            Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        std::vector<Apsis::Agent::Mover*> _movers;
      };
    }
  }
}

#endif

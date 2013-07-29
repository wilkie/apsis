#ifndef APSIS_AGENT_MOVERS_OR_H
#define APSIS_AGENT_MOVERS_OR_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <vector>

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Or : public Apsis::Agent::Mover {
      public:
        Or(Apsis::Agent::Mover* first,
           Apsis::Agent::Mover* second);

        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        std::vector<Apsis::Agent::Mover*> _movers;
      };
    }
  }
}

#endif

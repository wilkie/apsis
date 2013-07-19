#ifndef APSIS_AGENT_MOVERS_GRIDLOCK_DOWN_H
#define APSIS_AGENT_MOVERS_GRIDLOCK_DOWN_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <set>

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class GridlockDown : public Apsis::Agent::Mover {
      public:
        GridlockDown(Apsis::InputEngine& inputEngine);
        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
        virtual char* rule();
      private:
        InputEngine* _inputEngine;
      };
    }
  }
}

#endif

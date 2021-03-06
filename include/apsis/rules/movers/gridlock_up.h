#ifndef APSIS_AGENT_MOVERS_GRIDLOCK_UP_H
#define APSIS_AGENT_MOVERS_GRIDLOCK_UP_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <set>

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class GridlockUp : public Apsis::Agent::Mover {
      public:
        GridlockUp(Apsis::InputEngine& inputEngine);
        virtual bool update(float elapsed,
                            Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        InputEngine* _inputEngine;

        unsigned int _upAction;

        unsigned int _movingUpState;
        unsigned int _movingLeftState;
        unsigned int _movingRightState;
        unsigned int _movingDownState;
      };
    }
  }
}

#endif

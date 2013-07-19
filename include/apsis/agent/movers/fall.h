#ifndef APSIS_AGENT_MOVERS_FALL_H
#define APSIS_AGENT_MOVERS_FALL_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Fall : public Apsis::Agent::Mover {
      public:
        Fall(float startingVelocity,
             float acceleration,
             float terminalVelocity);

        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
        virtual char* rule();
      private:
        float _startingVelocity;
        float _acceleration;
        float _terminalVelocity;

        float _velocity;
      };
    }
  }
}

#endif

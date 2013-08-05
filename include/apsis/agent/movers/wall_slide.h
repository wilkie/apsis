#ifndef APSIS_AGENT_MOVERS_WALLSLIDE_H
#define APSIS_AGENT_MOVERS_WALLSLIDE_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class WallSlide : public Apsis::Agent::Mover {
      public:
        WallSlide(float startingVelocity,
                  float acceleration,
                  float terminalVelocity);

        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        float _startingVelocity;
        float _acceleration;
        float _terminalVelocity;

        float _velocity;

        unsigned int _collideWithLeftState;
        unsigned int _collideWithRightState;
        unsigned int _jumpingState;
      };
    }
  }
}

#endif

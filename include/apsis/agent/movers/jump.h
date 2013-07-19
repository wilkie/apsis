#ifndef APSIS_AGENT_MOVERS_JUMP_H
#define APSIS_AGENT_MOVERS_JUMP_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Jump : public Apsis::Agent::Mover {
      public:
        Jump(Apsis::InputEngine& inputEngine,
             float maximumHeight,
             float startingVelocity,
             float deacceleration,
             float peakDeacceleration,
             float minimumVelocity);
        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
        virtual char* rule();
      private:
        InputEngine* _inputEngine;

        float _maximumHeight;

        float _startingVelocity;
        float _deacceleration;
        float _peakDeacceleration;
        float _minimumVelocity;

        float _velocity;

        float _height;
      };
    }
  }
}

#endif

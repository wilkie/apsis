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
                            Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        InputEngine* _inputEngine;

        unsigned int _jumpAction;

        float _maximumHeight;

        float _startingVelocity;
        float _deacceleration;
        float _peakDeacceleration;
        float _minimumVelocity;

        float _velocity;

        float _height;

        unsigned int _collideWithLeftState;
        unsigned int _collideWithRightState;
        unsigned int _collideWithTopState;
        unsigned int _collideWithBottomState;
        unsigned int _jumpingState;
        unsigned int _canJumpState;
      };
    }
  }
}

#endif

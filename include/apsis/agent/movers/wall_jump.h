#ifndef APSIS_AGENT_MOVERS_WALL_JUMP_H
#define APSIS_AGENT_MOVERS_WALL_JUMP_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class WallJump : public Apsis::Agent::Mover {
      public:
        WallJump(Apsis::InputEngine& inputEngine,
                 float maximumHeight,
                 float startingVelocity,
                 float deacceleration,
                 float minimumVelocity,
                 float startingHorizontalVelocity,
                 float horizontalDeacceleration);

        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        InputEngine* _inputEngine;

        float _maximumHeight;

        float _startingVelocity;
        float _deacceleration;
        float _minimumVelocity;

        float _velocity;

        float _height;

        float _startingHorizontalVelocity;
        float _horizontalDeacceleration;

        float _horizontalVelocity;

        bool  _wallJumping;
        bool  _goingRight;

        unsigned int _collideWithLeftState;
        unsigned int _collideWithRightState;
        unsigned int _collideWithTopState;
        unsigned int _collideWithBottomState;
        unsigned int _jumpingState;
        unsigned int _canJumpState;

        unsigned int _jumpAction;
      };
    }
  }
}

#endif

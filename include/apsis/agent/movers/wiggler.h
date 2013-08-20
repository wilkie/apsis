#ifndef APSIS_AGENT_MOVERS_WIGGLER_H
#define APSIS_AGENT_MOVERS_WIGGLER_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Wiggler : public Apsis::Agent::Mover {
      public:
        Wiggler(float amount,
                float time,
                float stall);

        virtual bool update(float elapsed,
                            Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        float _amount;
        float _time;
        float _stall;

        float _travelled;
        float _direction;
      };
    }
  }
}

#endif
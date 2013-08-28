#ifndef APSIS_AGENT_MOVERS_LINEAR_H
#define APSIS_AGENT_MOVERS_LINEAR_H

#include "apsis/agent/mover.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Linear : public Apsis::Agent::Mover {
      public:
        Linear(float direction,
               float speed);

        virtual bool update(float elapsed,
                            Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
		float _direction;
        float _speed;
      };
    }
  }
}

#endif

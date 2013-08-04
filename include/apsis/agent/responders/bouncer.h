#ifndef APSIS_AGENT_RESPONDERS_BOUNCER_H
#define APSIS_AGENT_RESPONDERS_BOUNCER_H

#include "apsis/agent/responder.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Responders {
      class Bouncer : public Apsis::Agent::Responder {
      public:
        Bouncer(float direction,
                float speed);
		
        virtual bool uponEvent(std::set<unsigned int>& states,
                               Apsis::Geometry::Rectangle& original,
                               Apsis::Geometry::Point& intended);
      private:
		float _direction;
        float _speed;
      };
    }
  }
}

#endif

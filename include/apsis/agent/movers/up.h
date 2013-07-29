#ifndef APSIS_AGENT_MOVERS_UP_H
#define APSIS_AGENT_MOVERS_UP_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Up : public Apsis::Agent::Mover {
      public:
        Up(Apsis::InputEngine& inputEngine);
        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        InputEngine* _inputEngine;
      };
    }
  }
}

#endif

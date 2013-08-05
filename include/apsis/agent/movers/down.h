#ifndef APSIS_AGENT_MOVERS_DOWN_H
#define APSIS_AGENT_MOVERS_DOWN_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class Down : public Apsis::Agent::Mover {
      public:
        Down(Apsis::InputEngine& inputEngine,
             float speed);
        virtual bool update(float elapsed,
                            std::set<unsigned int>& states,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        InputEngine* _inputEngine;

        unsigned int _downAction;

        float _speed;
      };
    }
  }
}

#endif

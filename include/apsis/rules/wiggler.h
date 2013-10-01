#ifndef APSIS_RULES_WIGGLER_H
#define APSIS_RULES_WIGGLER_H

#include "apsis/rules/update_function.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

namespace Apsis {
namespace Rules {
    class Wiggler {
    public:
      static bool update(float elapsed,
                          const Apsis::World::Scene& scene,
                          Apsis::World::Object& object);
    };
  }
}

#endif
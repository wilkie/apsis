#ifndef APSIS_INTERFACE_LABEL_H
#define APSIS_INTERFACE_LABEL_H

#include "apsis/interface/window.h"
#include "apsis/world/object.h"

namespace Apsis {
  namespace Interface {
    class Label {
    public:
      static void draw(const Apsis::Geometry::Rectangle& position,
                       const Apsis::World::Object& object);
    };
  }
}

#endif
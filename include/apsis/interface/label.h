#ifndef APSIS_INTERFACE_LABEL_H
#define APSIS_INTERFACE_LABEL_H

#include "apsis/interface/window.h"

#include "apsis/world/object.h"

namespace Apsis {
  namespace Interface {
    class Label {
    public:
      static void init(const Apsis::Geometry::Rectangle& position,
                       Apsis::World::Object& object);

      static void draw(Apsis::Engine::Graphics& graphics,
                       const Apsis::Geometry::Rectangle& position,
                       const Apsis::World::Object& object);
    };
  }
}

#endif
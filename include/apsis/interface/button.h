#ifndef APSIS_INTERFACE_BUTTON_H
#define APSIS_INTERFACE_BUTTON_H

#include "apsis/interface/window.h"

#include "apsis/world/object.h"

namespace Apsis {
  namespace Interface {
    class Button {
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
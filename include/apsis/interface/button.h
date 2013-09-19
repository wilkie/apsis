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

      static void input(bool pressed,
                        const Apsis::Input::Binding&      binding,
                        const Apsis::Geometry::Point&     point,
                        const Apsis::Geometry::Rectangle& position,
                        Apsis::World::Object&             object);

      static void update(float elapsed,
                         Apsis::Geometry::Rectangle& position,
                         Apsis::World::Object& object);

      static void enter(const Apsis::Geometry::Point& point,
                        const Apsis::Geometry::Rectangle& position,
                        Apsis::World::Object& object);

      static void leave(const Apsis::Geometry::Point& point,
                        const Apsis::Geometry::Rectangle& position,
                        Apsis::World::Object& object);
    };
  }
}

#endif
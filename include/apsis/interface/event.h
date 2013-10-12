#ifndef APSIS_INTERFACE_EVENT_H
#define APSIS_INTERFACE_EVENT_H

#include "apsis/engine/graphics.h"
#include "apsis/engine/object.h"

#include "apsis/input/binding.h"

#include "apsis/geometry/rectangle.h"

#include "apsis/world/object.h"

namespace Apsis {
  namespace Interface {
    class Window;

    namespace Event {
      // Init function
      typedef void(Init)(const Apsis::Interface::Window& window,
                         Apsis::World::Object& object,
                         const Engine::Object& loader);

      // Draw function
      typedef void(Draw)(Apsis::Engine::Graphics&        graphics,
                         const Apsis::Interface::Window& window,
                         const Apsis::World::Object&     object);

      // Update function (for animations)
      typedef void(Update)(float elapsed,
                           Apsis::Geometry::Rectangle& position,
                           Apsis::World::Object&       object);

      // Input function
      typedef void(Input)(bool pressed,
                          const Apsis::Input::Binding&    binding,
                          const Apsis::Geometry::Point&   point,
                          const Apsis::Interface::Window& window,
                          Apsis::World::Object&           object);

      // Hover function
      typedef void(Motion)(const Apsis::Geometry::Point&   point,
                           const Apsis::Interface::Window& window,
                           Apsis::World::Object&           object);

      // When input pointer enters bounds
      typedef void(Enter)(const Apsis::Geometry::Point&   point,
                          const Apsis::Interface::Window& window,
                          Apsis::World::Object&           object);

      // When input pointer leaves bounds
      typedef void(Leave)(const Apsis::Geometry::Point&   point,
                          const Apsis::Interface::Window& window,
                          Apsis::World::Object&           object);

      void defaultInit(const Apsis::Interface::Window& window,
                       Apsis::World::Object& object,
                       const Engine::Object& loader);

      void defaultDraw(Apsis::Engine::Graphics&        graphics,
                       const Apsis::Interface::Window& window,
                       const Apsis::World::Object&     object);

      void defaultUpdate(float elapsed,
                         Apsis::Geometry::Rectangle& position,
                         Apsis::World::Object&       object);

      void defaultInput(bool pressed,
                        const Apsis::Input::Binding&    binding,
                        const Apsis::Geometry::Point&   point,
                        const Apsis::Interface::Window& window,
                        Apsis::World::Object&           object);

      void defaultMotion(const Apsis::Geometry::Point&   point,
                         const Apsis::Interface::Window& window,
                         Apsis::World::Object&           object);

      void defaultEnter(const Apsis::Geometry::Point&   point,
                        const Apsis::Interface::Window& window,
                        Apsis::World::Object&           object);

      void defaultLeave(const Apsis::Geometry::Point&   point,
                        const Apsis::Interface::Window& window,
                        Apsis::World::Object&           object);
    }
  }
}

#endif
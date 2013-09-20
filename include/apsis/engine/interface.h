#ifndef APSIS_ENGINE_INTERFACE_H
#define APSIS_ENGINE_INTERFACE_H

#include "apsis/engine/graphics.h"

#include "apsis/registry/interface.h"
#include "apsis/interface/window.h"

#include "apsis/geometry/point.h"
#include "apsis/input/binding.h"

#include <vector>
#include <map>

namespace Apsis {
  namespace Engine {
    class Interface {
    public:
      /*
       *  Constructs an interface engine to hold and manage interfaces.
       */
      static Apsis::Engine::Interface& basic(float width,
                                             float height);

      /*
       *  Adds the current Interface to the stack.
       */
      void push(const Apsis::Registry::Interface& iface);

      /*
       *  Removes the foreground Interface from the stack.
       */
      void pop();

      /*
       *  Returns the number of active interfaces.
       */
      unsigned int count() const;

      /*
       *  Removes the given Interface from the stack.
       */
      void pop(const Apsis::Registry::Interface& iface);

      /*
       *  Returns the current foreground Interface.
       */
      const Apsis::Registry::Interface& top() const;

      /*
       *  Removes all interfaces.
       */
      void clear();

      /*
       *  Draw with the given Graphics engine.
       */
      void draw(Engine::Graphics& graphics) const;

      /*
       *  Updates the interface given the amount of elapsed time.
       */
      void update(float elapsed);

      /*
       *  Tells the interface that there is some input interaction.
       */
      void input(bool pressed,
                 const Apsis::Geometry::Point& point,
                 const Apsis::Input::Binding& binding);

      /*
       *  Tells the interface that there is pointer movement across this
       *  Window.
       */
      void motion(const Apsis::Geometry::Point& point);

      /*
       *  Tells the interface that a pointer has entered the space.
       */
      void enter(const Apsis::Geometry::Point& point);

      /*
       *  Tells the interface that a pointer has left the space.
       */
      void leave(const Apsis::Geometry::Point& point);

    private:
      Interface(float width, float height);

      // Keeps track of Interface engines system-wide.
      static std::vector<Apsis::Engine::Interface*> _interface_engines;

      Apsis::Interface::Window _mainWindow;

      // Keep our own list of children by mapping the Interface id to the
      // window it spawned as an instance.
      std::map<unsigned int, Apsis::Interface::Window*> _windows;
      std::vector<const Apsis::Registry::Interface*> _interfaces;
    };
  }
}

#endif
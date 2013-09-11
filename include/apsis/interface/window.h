#ifndef APSIS_INTERFACE_WINDOW_H
#define APSIS_INTERFACE_WINDOW_H

#include "apsis/geometry/rectangle.h"
#include "apsis/world/object.h"

#include <stddef.h>

//Apsis::Graphics::Context
//position(position) <- push position to stack
//pop <- pop back to parent position
//position() <- get global position

//drawing functions?

//fonts?

//layout?

//caching?

namespace Apsis {
  namespace Interface {
    class Window {
    public:
      Window(float x,
             float y,
             float width,
             float height,
             void(&draw)(const Apsis::Geometry::Rectangle& position,
                         const Apsis::World::Object& object));

      /*
       *  Returns the position of this Window within its parent.
       */
      const Apsis::Geometry::Rectangle& position() const;

      /*
       *  Returns the next sibling Window.
       */
      const Apsis::Interface::Window& next() const;

      /*
       *  Returns the previous sibling Window.
       */
      const Apsis::Interface::Window& prev() const;

      /*
       *  Returns the first child Window.
       */
      const Apsis::Interface::Window& child() const;

      /*
       *  Returns the number of children.
       */
      unsigned int childCount() const;

      /*
       *  Adds the given Window as a child.
       */
      void add(Apsis::Interface::Window& window);

      /*
       *  Detaches this window from its parent.
       */
      void detach();

      /*
       *  Returns true if this Window has a parent.
       */
      bool attached() const;

      /*
       *  Draws this Window and all of its children.
       */
      void draw() const;

    private:
      // Dimensions
      Geometry::Rectangle _position;

      // List of other windows
      Interface::Window* _next;
      Interface::Window* _prev;

      // Children
      Interface::Window* _child;
      unsigned int _childCount;

      // Parent
      Interface::Window* _parent;

      Apsis::World::Object _object;

      // Draw function
      void(&_draw)(const Apsis::Geometry::Rectangle& position,
                   const Apsis::World::Object&       object);
    };
  }
}

#endif
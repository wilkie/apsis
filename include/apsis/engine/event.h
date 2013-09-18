#ifndef APSIS_ENGINE_EVENT_H
#define APSIS_ENGINE_EVENT_H

#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

#include "apsis/geometry/point.h"
#include "apsis/input/key.h"
#include "apsis/input/binding.h"

#include <vector>

namespace Apsis {
  namespace Engine {
    /*
     *  Contains an input event.
     */
    class Event {
    public:
      struct Type {
        enum Types {
          SystemEvent,
          Press,
          Release,
          Motion,
        };
      };

      struct SystemEvent {
        enum Types {
          Quit
        };
      };

      /*
       *  The type of event.
       */
      Type::Types type() const;

      /*
       *  Returns the system event identifier.
       */
      Apsis::Engine::Event::SystemEvent::Types systemEvent() const;

      /*
       *  Sets the system event. Also will set type to refer to such an
       *  event.
       */
      void systemEvent(Apsis::Engine::Event::SystemEvent::Types value);

      /*
       *  Returns the key binding associated with this event.
       */
      const Apsis::Input::Binding binding() const;

      /*
       *  Sets the event data to that of the given key binding. Will also set
       *  type to indicate the event is a key binding depending on whether it
       *  has been indicated that the key was pressed or released.
       */
      void binding(const Apsis::Input::Binding& binding,
                   float x,
                   float y,
                   bool pressed);

      /*
       *  Sets the event data to reflect a motion event of the given
       *  coordinates.
       */
      void motion(float x,
                  float y);

      /*
       *  Returns the affective coordinate of the event.
       */
      const Apsis::Geometry::Point& point() const;

      /*
       *  Returns the x position of the event.
       */
      float x() const;

      /*
       *  Returns the y position of the event.
       */
      float y() const;

      /*
       *  Returns whether or not the event is a response to input of any kind.
       */
      bool isInput() const;

      /*
       *  Returns whether or not the event is a response to the system.
       */
      bool isSystem() const;

      /*
       *  Returns whether or not the event is a motion event.
       */
      bool isMotion() const;

    private:
      Type::Types _type;

      union {
        Apsis::Engine::Event::SystemEvent::Types systemEvent;
        struct {
          Apsis::Key::Code code;
          bool shift;
          bool alt;
          bool control;
          Apsis::Geometry::Point point;
        } binding;
      } _payload;
    };
  };
}

#endif

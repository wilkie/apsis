#ifndef ISOTASTY_INPUT_HANDLER
#define ISOTASTY_INPUT_HANDLER

#include "iso-tasty/key.h"

#include <vector>

namespace IsoTasty {
  struct KeyBinding {
    Key::Code key;

    bool shift;
    bool control;
    bool alt;
  };

  struct Binding {
    const char* name;
    int         value;
    KeyBinding  primary;
    KeyBinding  secondary;
  };

  /*
   *  Handles mapping between keys and events allowing for flexible control
   *    over what keys or buttons are mapped to particular functions.
   */
  class InputHandler {
    public:

      /*
       *  Constructs a bare input handler that can be assigned to an
       *    Badger::Engine.
       */
      InputHandler();
      ~InputHandler();

      /*
       *  Creates a new binding entry with the specified name and the given
       *    defaults. Use NULL to specify no initial mapping. The parameter
       *    value will be returned when a key is translated.
       */
      void registerEvent(const char* name,
                         int         value,
                         KeyBinding* primary,
                         KeyBinding* secondary);

      /*
       *  Rebinds a key combination to the primary binding of the given event.
       *    The event must have been registered by registerEvent().
       */
      void rebindPrimary(const char* name,
                         KeyBinding* primary);

      /*
       *  Rebinds a key combination to the secondary binding of the given
       *    event. The event must have been registered by registerEvent().
       */
      void rebindSecondary(const char* name,
                           KeyBinding* secondary);

      int yieldEvent(bool pressed, KeyBinding* binding);

      bool isEventHeld(int event);

    private:
      // Stores all of the bindings
      std::vector<Binding*> _bindings;

      // Stores held events
      std::vector<int> _held;
  };

}

#endif

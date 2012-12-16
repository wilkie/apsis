#ifndef ISOTASTY_KEY_BINDING_REPOSITORY
#define ISOTASTY_KEY_BINDING_REPOSITORY

#include "iso-tasty/key.h"
#include "iso-tasty/key_binding.h"

#include <vector>

namespace IsoTasty {
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
  class KeyBindingRepository {
    public:

      /*
       *  Constructs a new collection of bindings.
       */
      KeyBindingRepository();
      ~KeyBindingRepository();

      /*
       *  Creates a new binding entry with the specified name and the given
       *    defaults. Use NULL to specify no initial mapping. The parameter
       *    event will be returned when a key is translated.
       */
      void registerEvent(const char* name,
                         int         event,
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

      /*
       *  Returns the event identifier that corresponds to the given key
       *    binding.
       */
      int yieldEvent(KeyBinding* binding);

    private:
      // Stores all of the bindings
      std::vector<Binding*> _bindings;
  };
}

#endif

#ifndef APSIS_INPUT_EVENT
#define APSIS_INPUT_EVENT

#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

#include "apsis/input/key.h"
#include "apsis/input/key_binding.h"
#include "apsis/input/key_binding_repository.h"

#include <vector>

namespace Apsis {
  /*
   *  Contains an input event.
   */
  struct Event {
    struct Type {
      enum Types {
        Quit,
        Input
      };
    };

    /*
     *  Whether or not the input event is depicting a pressed key or a
     *    released key.
     */
    bool pressed;

    /*
     *  The keybinding representing the keys pressed or released.
     */
    Apsis::KeyBinding binding;

    /*
     *  The type of event.
     */
    Type::Types type;
  };
}

#endif

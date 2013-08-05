#ifndef APSIS_INPUT_ENGINE
#define APSIS_INPUT_ENGINE

#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

#include "apsis/input/event.h"
#include "apsis/input/key.h"
#include "apsis/input/key_binding.h"
#include "apsis/input/key_binding_repository.h"

#include <vector>

namespace Apsis {
  /*
   *  Handles mapping between keys and events allowing for flexible control
   *    over what keys or buttons are mapped to particular functions.
   */
  class InputEngine {
    public:
      static const int QUIT_EVENT = -1;

      /*
       *  Constructs a bare input engine that can be assigned to an
       *    Apsis::Engine.
       */
      InputEngine();
      ~InputEngine();

      /*
       *  Returns the collection of key bindings. Use this class to manage
       *    the available key bindings.
       */
      KeyBindingRepository* keyBindings();

      /*
       *  Posts the input event and determines the bound Action identifier
       *  for any action that has been triggered. Returns true when an
       *  Action is triggered.
       */
      bool post(Apsis::Event& event, int& action);

      /*
       *  Returns true when the given event is pressed.
       */
      bool isEventHeld(int event);

    private:
      // Stores the key bindings
      KeyBindingRepository* _bindings;

      // Stores held events
      std::vector<int> _held;
  };

}

#endif

#ifndef ISOTASTY_INPUT_ENGINE
#define ISOTASTY_INPUT_ENGINE

#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

#include "iso-tasty/input/key.h"
#include "iso-tasty/input/key_binding.h"
#include "iso-tasty/input/key_binding_repository.h"

#include <vector>

namespace IsoTasty {
  /*
   *  Handles mapping between keys and events allowing for flexible control
   *    over what keys or buttons are mapped to particular functions.
   */
  class InputEngine {
    public:
      static const int QUIT_EVENT = -1;

      /*
       *  Constructs a bare input engine that can be assigned to an
       *    IsoTasty::Engine.
       */
      InputEngine();
      ~InputEngine();

      /*
       *  Returns the collection of key bindings. Use this class to manage
       *    the available key bindings.
       */
      KeyBindingRepository* keyBindings();

      /*
       *  Returns whether or not there are pending input requests.
       */
      int pollEvent();

      /*
       *  Simulates the given key binding press or release.
       */
      int forceEvent(bool pressed, KeyBinding* binding);

      /*
       *  Returns true when the given event is pressed.
       */
      bool isEventHeld(int event);

    private:
      // Stores the key bindings
      KeyBindingRepository* _bindings;

      // Stores held events
      std::vector<int> _held;

#ifndef NO_SDL
      /*
       *  Converts the given SDL_Event into a KeyBinding context.
       */
      static bool _translateSDLKey(IsoTasty::KeyBinding* binding, SDL_Event* event);

      /*
       *  Handles SDL input events.
       */
      int _SDLPollEvent();
#endif
  };

}

#endif

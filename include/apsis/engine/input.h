#ifndef APSIS_ENGINE_INPUT_H
#define APSIS_ENGINE_INPUT_H

#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

#include "apsis/engine/event.h"

#include "apsis/input/key.h"
#include "apsis/input/binding.h"

#include "apsis/registry/action.h"

#include <set>

namespace Apsis {
  namespace Engine {
    /*
     *  Handles which keys are pressed and can be polled to determine when
     *  an Action is still held.
     */
    class Input {
    public:
      /*
        *  Constructs a bare input engine.
        */
      static Apsis::Engine::Input& default();

      /*
        *  Tells the input engine to press the given key. It will return
        *  true when an action is triggered.
        */
      bool press(const Apsis::Input::Binding& binding,
                  unsigned int& action_id);

      /*
        *  Tells the input engine to release the given key. It will return
        *  true when an action is triggered.
        */
      bool release(const Apsis::Input::Binding& binding,
                    unsigned int& action_id);

      /*
        *  Returns true when the given action is still pressed.
        */
      bool isActionHeld(unsigned int action_id) const;

    private:
      /*
        *  Constructs a bare input engine that can be assigned to an
        *    Apsis::Engine.
        */
      Input();

      // Stores held events
      std::set<unsigned int> _held;

      // Keeps track of Object engines system-wide.
      static std::vector<Apsis::Engine::Input*> _input_engines;
    };
  }
}

#endif

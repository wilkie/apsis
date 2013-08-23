#ifndef APSIS_ENGINE_SYSTEM_H
#define APSIS_ENGINE_SYSTEM_H

#include "apsis/engine/input.h"

namespace Apsis {
  namespace Engine {
    class System {
    public:
      /*
       *  Constructs a new system. This is the main engine that coordinates
       *  the rest of the engines (input, audio, etc).
       */
      System();
      ~System();

      /*
       *  Returns the input engine for this system. This engine keeps
       *  track of input and defined bindings.
       */
      Apsis::Engine::Input& inputEngine();
      const Apsis::Engine::Input& inputEngine() const;

      /*
       *  Loads the Scene by the given unique id.
       */
      void loadScene(unsigned int id);

    private:
      Apsis::Engine::Input& _input;
    };
  }
}

#endif
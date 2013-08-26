#ifndef APSIS_ENGINE_SYSTEM_H
#define APSIS_ENGINE_SYSTEM_H

#include "apsis/engine/input.h"
#include "apsis/engine/object.h"

namespace Apsis {
  namespace Engine {
    /*
     *  This engine is the root of all resources.
     */
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
       *  Returns the object engine for this system. This engine keeps track
       *  of all of the loaded objects within the system.
       */
      Apsis::Engine::Object& objectEngine();
      const Apsis::Engine::Object& objectEngine() const;

      /*
       *  Loads the Scene by the given unique id.
       */
      void loadScene(unsigned int id);

    private:
      Apsis::Engine::Input&  _input;
      Apsis::Engine::Object& _objects;
    };
  }
}

#endif
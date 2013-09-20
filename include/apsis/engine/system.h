#ifndef APSIS_ENGINE_SYSTEM_H
#define APSIS_ENGINE_SYSTEM_H

#include "apsis/backend/sdl.h"

#include "apsis/engine/input.h"
#include "apsis/engine/object.h"
#include "apsis/engine/scene.h"
#include "apsis/engine/graphics.h"
#include "apsis/engine/audio.h"
#include "apsis/engine/interface.h"

#include "apsis/settings/video.h"
#include "apsis/settings/audio.h"

#include "apsis/interface/viewport.h"

#include "apsis/world/scene.h"

#include <json/json.h>

#include <string>

// TODO: INTERFACES!!!

// TODO: HANDLE DRAWING TO CANVASES, FONTS, ETC AND FIGURE OUT CACHING!!!

// TODO: BACKEND NEGOTIATION!!!

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
      System(const char* path,
             const Apsis::Settings::Video& videoSettings,
             const Apsis::Settings::Audio& audioSettings);

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
      const Apsis::Engine::Object& objectEngine() const;

      /*
       *  Transfers control over to this system until there is a quit event.
       */
      void run();

      /*
       *  Loads the Scene by the given unique id.
       */
      void loadScene(unsigned int id);

    private:
      // Backend
      Apsis::Backend::Sdl& _backend;

      // Scene Manager
      Apsis::Engine::Scene _scene;

      // Parses the given json via the path given in jsonFile.
      void _openJSONFile();

      // Parse JSON
      void _parseJSONFile();

      // Pulls Object engine parameters from JSON or creates from default.
      const Apsis::Engine::Object& _parseOrCreateObject();

      // Pulls Input engine and binding parameters from JSON or creates from
      // default.
      Apsis::Engine::Input&  _parseOrCreateInput();

      // Path to the JSON description.
      std::string _path;

      // Whether or not the JSON has been loaded
      bool _jsonLoaded;

      // JSON object
      Json::Value _value;

      // Input Engine
      Apsis::Engine::Input&  _input;

      // Object Engine
      const Apsis::Engine::Object& _objects;

      // Graphics Engine
      Apsis::Engine::Graphics& _graphics;

      // Audio Engine
      Apsis::Engine::Audio& _audio;

      // Interface Engine
      Apsis::Engine::Interface& _interface;

      // Screen Viewport
      Apsis::Interface::Viewport _viewport;
    };
  }
}

#endif
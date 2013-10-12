#include "apsis/engine/system.h"

#include "apsis/sync/clock.h"

#include <fstream>
#include <stdlib.h>

#ifdef JS_MODE
#include <emscripten.h>
#endif

Apsis::Engine::System::System(const char* path,
                              const Apsis::Settings::Video& videoSettings,
                              const Apsis::Settings::Audio& audioSettings)
  : _path(path),
    _jsonLoaded(false),

    // Initialize Devices
    _backend(*(new Apsis::Backend::Sdl(videoSettings))),

    // Initialize Input Engine
    _input(_parseOrCreateInput()),

    // Initialize Object Engine
    _objects(_parseOrCreateObject()),

    // Initialize Graphics Engine
    _graphics(Apsis::Engine::Graphics::basic(videoSettings)),

    // Initialize Shader Engine
    _shaders(Apsis::Engine::Shader::basic(_graphics)),

    // Initialize Audio Engine
    _audio(Apsis::Engine::Audio::basic(audioSettings)),

    // Initialize Scene Engine
    _scene(Apsis::Engine::Scene::basic()),

    // Initialize Interface Engine
    _interface(Apsis::Engine::Interface::basic((float)videoSettings.resolutionX,
                                               (float)videoSettings.resolutionY)),

    // Initialize Viewport
    _viewport(_scene,
              (float)videoSettings.resolutionX,
              (float)videoSettings.resolutionY) {

  // Parse objects and system data and populate data and caches.
  _parseJSONFile();
}

const Apsis::Engine::Input& Apsis::Engine::System::inputEngine() const {
  return _input;
}

Apsis::Engine::Input& Apsis::Engine::System::inputEngine() {
  return _input;
}

const Apsis::Engine::Object& Apsis::Engine::System::objectEngine() const {
  return _objects;
}

void Apsis::Engine::System::_openJSONFile() {
  if (_jsonLoaded) {
    return;
  }

  Json::Reader reader;

  std::ifstream file(_path);
  reader.parse(file, _value);
  file.close();

  _jsonLoaded = true;

  if (_value.isMember("inherit")) {
    // TODO: inherit games
  }
}

const Apsis::Engine::Object& Apsis::Engine::System::_parseOrCreateObject() {
  _openJSONFile();

  if (!(_value.isMember("type") &&
        _value["type"].compare(Json::Value("system")) == 0)) {
    throw "System description file given does not of type 'system'.";
  }

  if (_value.isMember("paths")) {
    if (_value["paths"].isObject()) {
      // Embedded
      return Apsis::Engine::Object::load(_value["paths"]);
    }
    else if (_value["paths"].isString()) {
      // Specified
      return Apsis::Engine::Object::load(_value["paths"].asCString());
    }
  }

  // Default
  return Apsis::Engine::Object::basic();
}

Apsis::Engine::Input& Apsis::Engine::System::_parseOrCreateInput() {
  _openJSONFile();

  // Default
  return Apsis::Engine::Input::basic();
}

void Apsis::Engine::System::_parseJSONFile() {
  _openJSONFile();

  // Load Scenes
  if (_value.isMember("scenes")) {
    if (!_value["scenes"].isArray()) {
      throw "System description file's 'scenes' member is not an array of strings.";
    }

    bool sceneLoaded = false;
    for (Json::Value::iterator it = _value["scenes"].begin();
         it != _value["scenes"].end();
         ++it) {
      if ((*it).isString()) {
        const Apsis::Registry::Scene& scene
          = _objects.loadScene((*it).asCString());
        if (!sceneLoaded) {
          _scene.scene(*(new Apsis::World::Scene(scene)));
          sceneLoaded = true;
        }
      }
      else {
        throw "A member of the system description file's 'scenes' is not a string.";
      }
    }
  }

  // Load key bindings
  if (_value.isMember("bindings")) {
    if (!_value["bindings"].isArray()) {
      throw "System description file's 'bindings' member is not an array of strings.";
    }

    for (Json::Value::iterator it = _value["bindings"].begin();
         it != _value["bindings"].end();
         ++it) {
      if ((*it).isString()) {
        _objects.loadBindings((*it).asCString());
      }
      else {
        throw "A member of the system description file's 'scenes' is not a string.";
      }
    }
  }
}

#ifdef JS_MODE
// Unsafe singleton pointer for C/js systems
static Apsis::Engine::System* _c_this;
#endif

void Apsis::Engine::System::run() {
  const Apsis::Registry::Interface& iface = _objects.loadInterface("hud");
  _interface.push(iface);

#ifdef JS_MODE
  _c_this = this;
  emscripten_set_main_loop(Apsis::Engine::System::_c_iterate, 60, 1);
#else
  while(_iterate()) {}
#endif

  _backend.quit();
}

void Apsis::Engine::System::_c_iterate() {
#ifdef JS_MODE
  _c_this->_iterate();
#endif
}

bool Apsis::Engine::System::_iterate() {
  static Apsis::Clock clock;

  Apsis::Engine::Event core_event;

  static Apsis::Geometry::Point cursor;
  static Apsis::Geometry::Point new_point;

  unsigned int action_id = 0;

  float elapsed = clock.elapsedTime();

  // TODO: Place this inside an interface engine or something.
  //       Remove the responsibility from the main viewport.
  // TODO: Rate limit events to keep up with drawing frame limit.
  while (_backend.poll(core_event)) {
    if (core_event.isInput()) {
      bool pressed = true;
      if (core_event.type() == Apsis::Engine::Event::Type::Release) {
        pressed = false;
      }

      const Apsis::Input::Binding& binding = core_event.binding();

      // Give interface the input
      _interface.input(pressed, core_event.point(), binding);

      // Give Scene the interaction if it is bound to an Action
      if (pressed && _input.press(binding, action_id)) {
        _scene.scene().act(action_id, true);
      }
      else if (!pressed && _input.release(binding, action_id)) {
        _scene.scene().act(action_id, false);
      }
    }
    else if (core_event.isMotion()) {
      new_point = core_event.point();
    }
    else if (core_event.isSystem()) {
      if (core_event.systemEvent() == Apsis::Engine::Event::SystemEvent::Quit) {
        return false;
      }
    }
  }

  // Give interface the motion input
  // Rate-limit motion events
  static float last = 0.0f;
  last += elapsed;
  if (last > 0.1f) {
    if (cursor.x != new_point.x || cursor.y != new_point.y) {
      cursor = new_point;
      _interface.motion(cursor);
    }
    last = fmod(last, 0.1f);
  }

  // TODO: Adjust the clock (cap minimum speed) to not
  //       under-simulate due to underflow?

  // Update scene actors
  _scene.scene().update(elapsed);

  // Update interface
  _interface.update(elapsed);

  // Draw scene
  _viewport.draw(_graphics);

  // Draw interface
  _interface.draw(_graphics);

  // Display
  _backend.swap();

  return true;
}

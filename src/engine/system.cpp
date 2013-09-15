#include "apsis/engine/system.h"

#include "apsis/sync/clock.h"

#include <fstream>
#include <stdlib.h>

Apsis::Engine::System::System(const char* path,
                              const Apsis::Settings::Video& videoSettings,
                              const Apsis::Settings::Audio& audioSettings)
  : _path(path),                                         // Set path
    _backend(*(new Apsis::Backend::Sdl(videoSettings))), // Initialize Devices
    _jsonLoaded(false),                                  // JSON
    _input(_parseOrCreateInput()),                       // Initialize Input Engine
    _objects(_parseOrCreateObject()),                    // Initialize Object Engine
    _graphics(Apsis::Engine::Graphics::basic(videoSettings)), // Initialize Graphics Engine
    _audio(Apsis::Engine::Audio::basic(audioSettings)),  // Initialize Audio Engine
    _scene(Apsis::Engine::Scene::basic()),               // Initialize Scene Engine
    _viewport(_scene,                                    // Initialize Viewport
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

void Apsis::Engine::System::run() {
  Apsis::Clock clock;

  Apsis::Engine::Event core_event;

  const Apsis::Audio::Music& music = Apsis::Audio::Music::load("assets/music/stabilizer_piraterap.ogg");
  _audio.play(music);

  unsigned int action_id = 0;
  while(true) {
    if (_backend.poll(core_event)) {
      if (core_event.type() == Apsis::Engine::Event::Type::Press) {
        if (_input.press(core_event.binding(), action_id)) {
          _scene.scene().act(action_id, true);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::Release) {
        if (_input.release(core_event.binding(), action_id)) {
          _scene.scene().act(action_id, false);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::SystemEvent) {
        if (core_event.systemEvent() == Apsis::Engine::Event::SystemEvent::Quit) {
          break;
        }
      }
    }
    _scene.scene().update(clock.elapsedTime());

    _viewport.draw(_graphics);

    _backend.swap();
  }

  _backend.quit();
}

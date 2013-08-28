#include "apsis/engine/system.h"

#include <fstream>

Apsis::Engine::System::System(const char* path)
  : _path(path),                           // Set path
    _jsonLoaded(false),                    // JSON
    _input(_parseOrCreateInput()),         // Initialize Input Engine
    _objects(_parseOrCreateObject()),
    _backend(Apsis::Backend::Sdl()) {     // Initialize Object Engine

  _backend.initialize(_videoSettings);
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

Apsis::Engine::Object& Apsis::Engine::System::objectEngine() {
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

void Apsis::Engine::System::_parseJSONFile() {
  _openJSONFile();
}

Apsis::Engine::Object& Apsis::Engine::System::_parseOrCreateObject() {
  _openJSONFile();

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
  return Apsis::Engine::Object::default();
}

Apsis::Engine::Input& Apsis::Engine::System::_parseOrCreateInput() {
  _openJSONFile();

  // Default
  return Apsis::Engine::Input::default();
}

void Apsis::Engine::System::run() {
  /*Apsis::Clock clock;

  Apsis::Engine::Event core_event;

  unsigned int action_id = 0;
  while(true) {
    if (_backend.poll(core_event)) {
      if (core_event.type() == Apsis::Engine::Event::Type::Press) {
        if (_input->press(core_event.binding(), action_id)) {
          _scene.act(action_id, true);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::Release) {
        if (_input->release(core_event.binding(), action_id)) {
          _scene.act(action_id, false);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::SystemEvent) {
        if (core_event.systemEvent() == Apsis::Engine::Event::SystemEvent::Quit) {
          break;
        }
      }
    }
    _scene.update(clock.elapsedTime());

    _draw();
    _backend.swap();
  }

  _backend.quit();*/
}
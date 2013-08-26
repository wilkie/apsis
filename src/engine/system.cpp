#include "apsis/engine/system.h"

#include <fstream>

Apsis::Engine::System::System(const char* path)
  : _path(path),                           // Set path
    _jsonLoaded(false),                    // JSON
    _input(*(new Apsis::Engine::Input())), // Initialize Input Engine
    _objects(_parseOrCreateObject()) {     // Initialize Object Engine
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
#include "apsis/engine/object.h"

#include <fstream>

std::vector<Apsis::Engine::Object*> Apsis::Engine::Object::_object_engines;

Apsis::Engine::Object& Apsis::Engine::Object::load(const char* path) {
  Apsis::Engine::Object* oe = new Apsis::Engine::Object(path);
  _object_engines.push_back(oe);
  return *oe;
}

Apsis::Engine::Object& Apsis::Engine::Object::load(Json::Value& value) {
  Apsis::Engine::Object* oe = new Apsis::Engine::Object(value);
  _object_engines.push_back(oe);
  return *oe;
}

Apsis::Engine::Object& Apsis::Engine::Object::default() {
  Apsis::Engine::Object* oe = new Apsis::Engine::Object();
  _object_engines.push_back(oe);
  return *oe;
}

Apsis::Engine::Object::Object() {
  _loadDefaults();
}

Apsis::Engine::Object::Object(const char* path) {
  Json::Value value;

  Json::Reader reader;

  std::ifstream file(path);
  reader.parse(file, value);
  file.close();

  _loadDefaults();
  if (value.isMember("type") &&
      value["type"].isString() &&
      value["type"].compare(Json::Value("paths"))) {
  _loadFromJSON(value);
  }
  else {
    throw "Object JSON should be of type 'paths'";
  }
}

Apsis::Engine::Object::Object(Json::Value& value) {
  _loadDefaults();
  _loadFromJSON(value);
}

void Apsis::Engine::Object::_loadFromJSON(Json::Value& value) {
  if (value.isMember("scenes")) {
    _scene_path = value["scenes"].asCString();
  }

  if (value.isMember("things")) {
    _thing_path = value["things"].asCString();
  }

  if (value.isMember("maps")) {
    _map_path = value["maps"].asCString();
  }

  if (value.isMember("bindings")) {
    _binding_path = value["bindings"].asCString();
  }

  if (value.isMember("graphics")) {
    _graphics_path = value["graphics"].asCString();
  }

  if (value.isMember("backgrounds")) {
    _background_path = value["backgrounds"].asCString();
  }

  if (value.isMember("rule")) {
    _rule_path = value["rule"].asCString();
  }
}

void Apsis::Engine::Object::_loadDefaults() {
  _scene_path = "";
  _thing_path = "";
  _map_path = "";
  _binding_path = "";
  _graphics_path = "";
  _background_path = "";
  _rule_path = "";
}
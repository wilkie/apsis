#include "apsis/engine/object.h"

#include <fstream>
#include <sys/stat.h>

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
  // TODO: Cleanup
  if (value.isMember("scenes")) {
    _scene_path = value["scenes"].asCString();
    if (_scene_path.size() > 0 &&
        _scene_path[_scene_path.size() - 1] != '/') {
      _scene_path.append("/");
    }
  }

  if (value.isMember("things")) {
    _thing_path = value["things"].asCString();
    if (_thing_path.size() > 0 &&
        _thing_path[_thing_path.size() - 1] != '/') {
      _thing_path.append("/");
    }
  }

  if (value.isMember("maps")) {
    _map_path = value["maps"].asCString();
    if (_map_path.size() > 0 &&
        _map_path[_map_path.size() - 1] != '/') {
      _map_path.append("/");
    }
  }

  if (value.isMember("bindings")) {
    _binding_path = value["bindings"].asCString();
    if (_binding_path.size() > 0 &&
        _binding_path[_binding_path.size() - 1] != '/') {
      _binding_path.append("/");
    }
  }

  if (value.isMember("graphics")) {
    _graphics_path = value["graphics"].asCString();
    if (_graphics_path.size() > 0 &&
        _graphics_path[_graphics_path.size() - 1] != '/') {
      _graphics_path.append("/");
    }
  }

  if (value.isMember("backgrounds")) {
    _background_path = value["backgrounds"].asCString();
    if (_background_path.size() > 0 &&
        _background_path[_background_path.size() - 1] != '/') {
      _background_path.append("/");
    }
  }

  if (value.isMember("rule")) {
    _rule_path = value["rule"].asCString();
    if (_rule_path.size() > 0 &&
        _rule_path[_rule_path.size() - 1] != '/') {
      _rule_path.append("/");
    }
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

const Apsis::Sprite::Thing& Apsis::Engine::Object::loadThing(const char* name) const {
  std::string found = _findFile(_thing_path, std::string(name));
  if (found == "") {
    throw "Thing description not found or loaded.";
  }
  return Apsis::Sprite::Thing::load(found.c_str());
}

const Apsis::World::Map& Apsis::Engine::Object::loadMap(const char* name) const {
  std::string found = _findFile(_map_path, std::string(name));
  if (found == "") {
    throw "Map description not found or loaded.";
  }
  return Apsis::World::Map::load(found.c_str(), *this);
}

const Apsis::Registry::Rule& Apsis::Engine::Object::loadRule(const char* name) const {
  std::string found = _findFile(_rule_path, std::string(name));
  if (found == "") {
    throw "Rule description not found or loaded.";
  }
  return Apsis::Registry::Rule::load(found.c_str());
}

const Apsis::Registry::Scene& Apsis::Engine::Object::loadScene(const char* name) const {
  std::string found = _findFile(_scene_path, std::string(name));
  if (found == "") {
    throw "Scene description not found or loaded.";
  }
  return Apsis::Registry::Scene::load(found.c_str(), *this);
}

std::string Apsis::Engine::Object::_findFile(const std::string& searchPath, const std::string& name) const {
  // TODO: We could actually cache the result of the file search
  //       with a map on searchPath and name.
  //       Then we could have an object cache that uses the unique id.

  // Add .json extension
  std::string search = name;
  search = search.append(".json");

  // Find json description file
  // Look in thing path first (if exists)
  if (searchPath.size() != 0) {
    std::string path = searchPath;
    path = path.append(search);

    if (_fileExists(path)) {
      return path;
    }
  }

  // If path search fails, search from project root.
  if (_fileExists(search)) {
    return search;
  }

  // Failure, return empty string. React elsewhere.
  return "";
}

bool Apsis::Engine::Object::_fileExists(std::string& path) const {
  struct stat info;
  int ret = -1;

  // If stat() is successful, the file exists and is readable.
  ret = stat(path.c_str(), &info);
  return ret == 0;
}
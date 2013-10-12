#include "apsis/engine/object.h"
#include "apsis/registry/interface.h"

#include "apsis/registry/scene.h"
#include "apsis/registry/action.h"
#include "apsis/registry/rule.h"
#include "apsis/registry/property.h"
#include "apsis/registry/state.h"
#include "apsis/registry/interface.h"
#include "apsis/registry/widget.h"
#include "apsis/registry/shader.h"
#include "apsis/registry/program.h"

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/thing.h"

#include "apsis/world/map.h"

#include <fstream>
#include <sys/stat.h>

using namespace Apsis;

std::vector<Engine::Object*> Engine::Object::_object_engines;

Engine::Object& Engine::Object::load(const char* path) {
  Engine::Object* oe = new Engine::Object(path);
  _object_engines.push_back(oe);
  return *oe;
}

Engine::Object& Engine::Object::load(Json::Value& value) {
  Engine::Object* oe = new Engine::Object(value);
  _object_engines.push_back(oe);
  return *oe;
}

Engine::Object& Engine::Object::basic() {
  Engine::Object* oe = new Engine::Object();
  _object_engines.push_back(oe);
  return *oe;
}

Engine::Object::Object() {
  _loadDefaults();
}

Engine::Object::Object(const char* path) {
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

Engine::Object::Object(Json::Value& value) {
  _loadDefaults();
  _loadFromJSON(value);
}

void Engine::Object::_loadFromJSON(Json::Value& value) {
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

  if (value.isMember("rules")) {
    _rule_path = value["rules"].asCString();
    if (_rule_path.size() > 0 &&
        _rule_path[_rule_path.size() - 1] != '/') {
      _rule_path.append("/");
    }
  }

  if (value.isMember("interfaces")) {
    _interface_path = value["interfaces"].asCString();
    if (_interface_path.size() > 0 &&
        _interface_path[_interface_path.size() - 1] != '/') {
      _interface_path.append("/");
    }
  }

  if (value.isMember("shaders")) {
    _shader_path = value["shaders"].asCString();
    if (_shader_path.size() > 0 &&
        _shader_path[_shader_path.size() - 1] != '/') {
      _shader_path.append("/");
    }
  }

  if (value.isMember("programs")) {
    _program_path = value["programs"].asCString();
    if (_program_path.size() > 0 &&
        _program_path[_shader_path.size() - 1] != '/') {
      _program_path.append("/");
    }
  }

  if (value.isMember("widgets")) {
    _widget_path = value["widgets"].asCString();
    if (_widget_path.size() > 0 &&
        _widget_path[_widget_path.size() - 1] != '/') {
      _widget_path.append("/");
    }
  }
}

void Engine::Object::_loadDefaults() {
  _scene_path = "";
  _thing_path = "";
  _map_path = "";
  _binding_path = "";
  _graphics_path = "";
  _background_path = "";
  _rule_path = "";
  _interface_path = "";
  _widget_path = "";
  _shader_path = "";
  _program_path = "";
}

const Sprite::Thing& Engine::Object::loadThing(const char* name) const {
  std::string found = _findFile(_thing_path, std::string(name));
  if (found == "") {
    throw "Thing description not found or loaded.";
  }
  return Sprite::Thing::load(found.c_str(), *this);
}

const World::Map& Engine::Object::loadMap(const char* name) const {
  std::string found = _findFile(_map_path, std::string(name));
  if (found == "") {
    throw "Map description not found or loaded.";
  }
  return Apsis::World::Map::load(found.c_str(), *this);
}

const Sprite::Sheet& Engine::Object::loadSheet(const char* name) const {
  std::string found = _findFile(_graphics_path, std::string(name));
  if (found == "") {
    char foo[1024];
    sprintf(foo, "Sheet description not found or loaded: %s", name);
    throw foo;
  }
  return Sprite::Sheet::load(found.c_str(), *this);
}

const Registry::Rule& Engine::Object::loadRule(const char* name) const {
  std::string found = _findFile(_rule_path, std::string(name));
  if (found == "") {
    throw "Rule description not found or loaded.";
  }
  return Apsis::Registry::Rule::load(found.c_str());
}

const Registry::Scene& Engine::Object::loadScene(const char* name) const {
  std::string found = _findFile(_scene_path, std::string(name));
  if (found == "") {
    throw "Scene description not found or loaded.";
  }
  return Registry::Scene::load(found.c_str(), *this);
}

const Registry::Interface& Engine::Object::loadInterface(const char* name) const {
  std::string found = _findFile(_interface_path, std::string(name));
  if (found == "") {
    throw "Interface description not found or loaded.";
  }
  return Registry::Interface::load(found.c_str(), *this);
}

const Registry::Widget& Engine::Object::loadWidget(const char* name) const {
  std::string found = _findFile(_widget_path, std::string(name));
  if (found == "") {
    throw "Widget description not found or loaded.";
  }
  return Registry::Widget::load(found.c_str(), *this);
}

void Engine::Object::loadBindings(const char* name) const {
  std::string found = _findFile(_binding_path, std::string(name));
  if (found == "") {
    throw "Bindings file not found or loaded.";
  }
  return Registry::Action::load(found.c_str());
}

const Registry::Shader& Engine::Object::loadShader(const char* name) const {
  std::string found = _findFile(_shader_path, std::string(name));
  if (found == "") {
    throw "Shader file not found or loaded.";
  }
  return Registry::Shader::load(found.c_str(), *this);
}

const Registry::Program& Engine::Object::loadProgram(const char* name) const {
  std::string found = _findFile(_program_path, std::string(name));
  if (found == "") {
    throw "Program file not found or loaded.";
  }
  return Registry::Program::load(found.c_str(), *this);
}

std::string Engine::Object::_findFile(const std::string& searchPath, const std::string& name) const {
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

bool Engine::Object::_fileExists(std::string& path) const {
  struct stat info;
  int ret = -1;

  // If stat() is successful, the file exists and is readable.
  ret = stat(path.c_str(), &info);
  return ret == 0;
}

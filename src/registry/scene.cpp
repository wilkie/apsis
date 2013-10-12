#include "apsis/registry/scene.h"

#include "apsis/engine/log.h"

#include <algorithm>
#include <fstream>

std::vector<std::string> Apsis::Registry::Scene::_ids;
std::vector<Apsis::Registry::Scene*> Apsis::Registry::Scene::_scenes;

const Apsis::Registry::Scene& Apsis::Registry::Scene::load(const char* path,
                                                           const Apsis::Engine::Object& loader) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_scenes[std::distance(_ids.begin(), it)];
  }

  Engine::Log::printf("Loading scene %s\n", path);
  _scenes.push_back(new Apsis::Registry::Scene(path, loader));
  _ids.push_back(str);
  return *_scenes[_ids.size() - 1];
}

Apsis::Registry::Scene::Scene(const char* path,
                              const Apsis::Engine::Object& loader) {
  Json::Reader reader;
  Json::Value  value;

  std::ifstream file(path);
  reader.parse(file, value);
  file.close();

  if (value.isMember("type") &&
      value["type"].compare(Json::Value("scene")) == 0) {
  }
  else {
    throw "Scene file given is not of type 'scene'.";
  }

  if (value.isMember("inherit")) {
    // TODO: inherit scenes
  }

  if (value.isMember("actors")) {
    // Load actors
    for (Json::Value::iterator it = value["actors"].begin();
         it != value["actors"].end();
         ++it) {
      if ((*it).isObject()) {
        if ((*it).isMember("thing") &&
            (*it).isMember("x") &&
            (*it).isMember("y")) {
          ActorInfo actor_info;

          actor_info.thing = &loader.loadThing((*it)["thing"].asCString());
          actor_info.x = (float)((*it)["x"].asDouble());
          actor_info.y = (float)((*it)["y"].asDouble());

          _actors.push_back(actor_info);
        }
      }
      else {
        throw "Scene file's 'actors' section is malformed.";
      }
    }
  }

  if (value.isMember("map")) {
    // Just one map
    if (value.isMember("maps")) {
      // Error
      throw "Scene file has both 'map' and 'maps' which is not allowed.";
    }

    MapInfo map_info;

    if (value["map"].isString()) {
      // x and y are 0
      map_info.x = 0;
      map_info.y = 0;
      map_info.map = &loader.loadMap(value["map"].asCString());
    }
    else if (value["map"].isMember("map") &&
             value["map"].isMember("x") &&
             value["map"].isMember("y")) {
      // x and y are given
      map_info.x = (float)(value["map"]["x"].asDouble());
      map_info.y = (float)(value["map"]["y"].asDouble());
      map_info.map = &loader.loadMap(value["map"]["map"].asCString());
    }

    _maps.push_back(map_info);
  }
  else if (value.isMember("maps")) {
    // More than one map
    for (Json::Value::iterator it = value["maps"].begin();
         it != value["maps"].end();
         ++it) {
      if ((*it).isMember("map") &&
          (*it).isMember("x") &&
          (*it).isMember("y")) {
        // x and y are given
        MapInfo map_info;
        map_info.x = (float)((*it)["x"].asDouble());
        map_info.y = (float)((*it)["y"].asDouble());
        map_info.map = &Apsis::World::Map::load((*it)["map"].asCString(),
                                                loader);
        _maps.push_back(map_info);
      }
      else {
        throw "Scene file's 'maps' section is malformed.";
      }
    }
  }
}

unsigned int Apsis::Registry::Scene::actorCount() const {
  return _actors.size();
}

const Apsis::Sprite::Thing& Apsis::Registry::Scene::actor(unsigned int index) const {
  return *_actors[index].thing;
}

float Apsis::Registry::Scene::actorX(unsigned int index) const {
  return _actors[index].x;
}

float Apsis::Registry::Scene::actorY(unsigned int index) const {
  return _actors[index].y;
}

unsigned int Apsis::Registry::Scene::mapCount() const {
  return _maps.size();
}

const Apsis::World::Map& Apsis::Registry::Scene::map(unsigned int index) const {
  return *_maps[index].map;
}

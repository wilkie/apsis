#include "apsis/world/scene.h"

#include <algorithm>

std::vector<std::string> Apsis::World::Scene::_ids;
std::vector<Apsis::World::Scene*> Apsis::World::Scene::_scenes;

Apsis::World::Scene::Scene() {
}

Apsis::World::Scene::Scene(const char* path) {
  // TODO: load from JSON and Object Engine
}

Apsis::World::Scene::Scene(const Apsis::Registry::Scene& scene) {
  for (unsigned int i = 0; i < scene.actorCount(); i++) {
    addActor(scene.actor(i), scene.actorX(i), scene.actorY(i));
  }

  for (unsigned int i = 0; i < scene.mapCount(); i++) {
    addMap(scene.map(i));
  }
}

void Apsis::World::Scene::addMap(const Apsis::World::Map& map) {
  _maps.push_back(&map);
}

unsigned int Apsis::World::Scene::addActor(unsigned int id, float x, float y) {
  _actors.push_back(Apsis::World::Actor(Apsis::Sprite::Thing::get(id), (unsigned int)x, (unsigned int )y));
  return _actors.size() - 1;
}

void Apsis::World::Scene::update(float elapsed) {
  for(std::vector<Apsis::World::Actor>::iterator it = _actors.begin();
      it != _actors.end();
      ++it) {

    (*it).update(*this, elapsed);
  }

  for(std::vector<Apsis::World::Actor>::iterator it = _actors.begin();
      it != _actors.end();
      ++it) {

    (*it).collide(*this);
  }

  for(std::vector<Apsis::World::Actor>::iterator it = _actors.begin();
      it != _actors.end();
      ++it) {

    (*it).respond(*this);
  }
}

void Apsis::World::Scene::act(unsigned int action_id, bool held) {
  for(std::vector<Apsis::World::Actor>::iterator it = _actors.begin();
      it != _actors.end();
      ++it) {

    (*it).act(*this, action_id, held);
  }
}

unsigned int Apsis::World::Scene::mapCount() const {
  return _maps.size();
}

unsigned int Apsis::World::Scene::actorCount() const {
  return _actors.size();
}

unsigned int Apsis::World::Scene::addActor(const Apsis::Sprite::Thing& thing, float x, float y) {
  _actors.push_back(Apsis::World::Actor(thing, (unsigned int)x, (unsigned int)y));
  return _actors.size() - 1;
}

void Apsis::World::Scene::draw(const glm::mat4& projection,
                               Primitives::Camera& camera,
                               const glm::mat4& model) const {
  for(std::vector<const Apsis::World::Map*>::const_iterator it = _maps.begin();
      it != _maps.end();
      ++it) {
    (*it)->draw(projection, camera, model);
  }

  for(std::vector<Apsis::World::Actor>::const_iterator it = _actors.begin();
      it != _actors.end();
      ++it) {
    (*it).draw(projection, camera);
  }
}

const Apsis::World::Map& Apsis::World::Scene::map(unsigned int id) const {
  return *_maps[id];
}

Apsis::World::Actor& Apsis::World::Scene::actor(unsigned int id) {
  return _actors[id];
}

const Apsis::World::Actor& Apsis::World::Scene::actor(unsigned int id) const {
  return _actors[id];
}

const Apsis::World::Scene& Apsis::World::Scene::load(const char* path) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_scenes[std::distance(_ids.begin(), it)];
  }

  _scenes.push_back(new Apsis::World::Scene(path));
  _ids.push_back(str);
  return *_scenes[_ids.size() - 1];
}
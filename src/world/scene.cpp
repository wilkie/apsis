#include "apsis/world/scene.h"

Apsis::World::Scene::Scene() {
}

void Apsis::World::Scene::addMap(Apsis::World::Map& map) {
  _maps.push_back(map);
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
  for(std::vector<Apsis::World::Map>::const_iterator it = _maps.begin();
      it != _maps.end();
      ++it) {
    (*it).draw(projection, camera, model);
  }

  for(std::vector<Apsis::World::Actor>::const_iterator it = _actors.begin();
      it != _actors.end();
      ++it) {
    (*it).draw(projection, camera);
  }
}

Apsis::World::Map& Apsis::World::Scene::map(unsigned int id) {
  return _maps[id];
}

const Apsis::World::Map& Apsis::World::Scene::map(unsigned int id) const {
  return _maps[id];
}

Apsis::World::Actor& Apsis::World::Scene::actor(unsigned int id) {
  return _actors[id];
}

const Apsis::World::Actor& Apsis::World::Scene::actor(unsigned int id) const {
  return _actors[id];
}
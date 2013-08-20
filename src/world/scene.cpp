#include "apsis/world/scene.h"

Apsis::World::Scene::Scene() {
}

void Apsis::World::Scene::addMap(Apsis::World::Map& map) {
  _maps.push_back(map);
}

unsigned int Apsis::World::Scene::addActor(unsigned int id, float x, float y) {
  _actors.push_back(Apsis::World::Actor(Apsis::Sprite::Thing::get(id), (float)x, (float)y));
  return _actors.size() - 1;
}

void Apsis::World::Scene::update(float elapsed) {
  for(std::vector<Apsis::World::Actor>::iterator it = _actors.begin();
      it != _actors.end();
      ++it) {
    (*it).update(elapsed);
  }
}

unsigned int Apsis::World::Scene::addActor(const Apsis::Sprite::Thing& thing, float x, float y) {
  _actors.push_back(Apsis::World::Actor(thing, (float)x, (float)y));
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

Apsis::World::Actor& Apsis::World::Scene::actor(unsigned int id) {
  return _actors[id];
}
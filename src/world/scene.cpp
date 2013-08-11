#include "apsis/world/scene.h"

Apsis::World::Scene::Scene() {
}

void Apsis::World::Scene::addMap(Apsis::World::Map& map) {
  _maps.push_back(map);
}

unsigned int Apsis::World::Scene::addActor(unsigned int id, float x, float y) {
  _actors.push_back(Apsis::World::Actor(Apsis::Sprite::Thing::get(id), 0, 0));
  return _actors.size() - 1;
}
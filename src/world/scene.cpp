#include "apsis/world/scene.h"

Apsis::World::Scene::Scene() {
}

void Apsis::World::Scene::addMap(Apsis::World::Map& map) {
  _maps.push_back(map);
}

unsigned int Apsis::World::Scene::addActor(unsigned int id) {
  _actors.push_back(Apsis::Registry::Actor::clone(id));
  return _actors.size() - 1;
}
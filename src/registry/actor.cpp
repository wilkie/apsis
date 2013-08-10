#include <apsis/registry/actor.h>

#include <algorithm>

std::vector<std::string> Apsis::Registry::Actor::_ids;
std::vector<Apsis::World::Actor> Apsis::Registry::Actor::_actors;

unsigned int Apsis::Registry::Actor::id(const char* path) {
  Apsis::World::Actor actor(path, 0, 0);

  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return std::distance(_ids.begin(), it);
  }

  _ids.push_back(str);
  _actors.push_back(actor);

  return _ids.size() - 1;
}

Apsis::World::Actor& Apsis::Registry::Actor::actor(const char* path) {
  return Apsis::Registry::Actor::actor(Apsis::Registry::Actor::id(path));
}

Apsis::World::Actor& Apsis::Registry::Actor::actor(unsigned int id) {
  return _actors[id];
}

Apsis::World::Actor Apsis::Registry::Actor::clone(const char* path) {
  return Apsis::Registry::Actor::clone(Apsis::Registry::Actor::id(path));
}

Apsis::World::Actor Apsis::Registry::Actor::clone(unsigned int id) {
  return _actors[id];
}
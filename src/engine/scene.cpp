#include "apsis/engine/scene.h"

std::vector<Apsis::Engine::Scene*> Apsis::Engine::Scene::_scene_engines;

Apsis::Engine::Scene& Apsis::Engine::Scene::basic() {
  Apsis::Engine::Scene* se = new Apsis::Engine::Scene();
  _scene_engines.push_back(se);
  return *se;
}

Apsis::Engine::Scene::Scene()
  : _scene(NULL) {
}

Apsis::World::Scene& Apsis::Engine::Scene::scene() {
  if (_scene == NULL) {
    throw "Engine::Scene Error: No scene has been loaded.";
  }
  return *_scene;
}

const Apsis::World::Scene& Apsis::Engine::Scene::scene() const {
  if (_scene == NULL) {
    throw "Engine::Scene Error: No scene has been loaded.";
  }
  return *_scene;
}

void Apsis::Engine::Scene::scene(Apsis::World::Scene& scene) {
  _scene = &scene;
}

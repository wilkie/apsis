#ifndef APSIS_WORLD_SCENE_H
#define APSIS_WORLD_SCENE_H

#include "apsis/world/map.h"
#include "apsis/world/actor.h"

#include "apsis/registry/actor.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Scene {
    public:
      /*
       *  Creates a new Scene.
       */
      Scene();

      /*
       *  Adds a Map to the scene.
       */
      void addMap(Apsis::World::Map& map);

      /*
       *  Instantiates an Actor to the scene.
       */
      unsigned int addActor(unsigned int id);
    private:
      // Collections of objects within the scene
      std::vector<Apsis::World::Actor> _actors;
      std::vector<Apsis::World::Map>   _maps;
    };
  }
}

#endif
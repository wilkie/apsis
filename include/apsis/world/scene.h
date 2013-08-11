#ifndef APSIS_WORLD_SCENE_H
#define APSIS_WORLD_SCENE_H

#include "apsis/world/map.h"
#include "apsis/world/actor.h"

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
       *  Instantiates an Actor to the scene using a Thing id.
       */
      unsigned int addActor(unsigned int id, float x, float y);

      /*
       *  Instantiates an Actor to the scene by cloning the given Thing.
       */
      unsigned int addActor(Sprite::Thing& thing, float x, float y);
    private:
      // Collections of objects within the scene
      std::vector<Apsis::World::Actor> _actors;
      std::vector<Apsis::World::Map>   _maps;
    };
  }
}

#endif
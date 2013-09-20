#ifndef APSIS_REGISTRY_SCENE_H
#define APSIS_REGISTRY_SCENE_H

#include "apsis/engine/object.h"

#include "apsis/sprite/thing.h"
#include "apsis/world/map.h"

#include <json/json.h>

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class Scene {
    public:
      static const Apsis::Registry::Scene& load(const char* path,
                                                const Apsis::Engine::Object& loader);

      unsigned int actorCount() const;

      const Apsis::Sprite::Thing& actor(unsigned int index) const;
      float actorX(unsigned int index) const;
      float actorY(unsigned int index) const;

      unsigned int mapCount() const;

      const Apsis::World::Map& map(unsigned int index) const;

    private:
      // Constructors
      Scene(const char* path,
            const Apsis::Engine::Object& loader);

      // Keeps track of Scenes system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::Registry::Scene*> _scenes;

      struct ActorInfo {
        const Apsis::Sprite::Thing* thing;
        float x;
        float y;
      };

      struct MapInfo {
        const Apsis::World::Map* map;
        float x;
        float y;
      };

      std::vector<ActorInfo> _actors;
      std::vector<MapInfo>   _maps;
    };
  }
}

#endif
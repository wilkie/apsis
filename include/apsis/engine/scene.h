#ifndef APSIS_ENGINE_SCENE_H
#define APSIS_ENGINE_SCENE_H

#include "apsis/world/scene.h"

namespace Apsis {
  namespace Engine {
    /*
     *  This class manages the current scene.
     */
    class Scene {
    public:
      /*
       *  Constructs a scene engine to hold and manage the scene.
       */
      static Apsis::Engine::Scene& default();

      /*
       *  Sets the scene.
       */
      void scene(Apsis::World::Scene& scene);

      /*
       *  Returns the current scene.
       */
      Apsis::World::Scene& scene();
      const Apsis::World::Scene& scene() const;

    private:
      Scene();

      Apsis::World::Scene* _scene;

      // Keeps track of Scene engines system-wide.
      static std::vector<Apsis::Engine::Scene*> _scene_engines;
    };
  }
}

#endif
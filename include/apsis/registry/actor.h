#ifndef APSIS_REGISTRY_ACTOR_H
#define APSIS_REGISTRY_ACTOR_H

#include "apsis/world/actor.h"

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all unique actors that exist in the system.
     *  Actors are an object in the system that can act upon the scene or
     *  itself. Actors in this registry can be cloned to be added to a scene.
     */
    class Actor {
    public:
      /*
       *  Reads and registers or simply returns the existing id for the given
       *  actor defined by the actor file at the given path.
       */
      static unsigned int id(const char* path);
      
      static Apsis::World::Actor& actor(const char* path);
      static Apsis::World::Actor& actor(unsigned int id);

      static Apsis::World::Actor clone(const char* path);
      static Apsis::World::Actor clone(unsigned int id);
    private:
      static std::vector<std::string> _ids;
      static std::vector<Apsis::World::Actor> _actors;
    };
  }
}

#endif APSIS_REGISTRY_ACTOR_H
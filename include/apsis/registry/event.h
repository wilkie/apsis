#ifndef APSIS_REGISTRY_EVENT_H
#define APSIS_REGISTRY_EVENT_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all events that exist in the system. Events are
     *  triggers that occur when conditions are met. For instance, there may
     *  be an event for when a player character collides with a projectile.
     *  In this case, the event can be responded to in turn by rule code that
     *  will subtract health from the player and trigger a knockback state.
     */
    class Event {
    public:
      /*
       *  Registers or returns the existing id for the given event.
       */
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
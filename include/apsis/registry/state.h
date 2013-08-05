#ifndef APSIS_REGISTRY_STATE_H
#define APSIS_REGISTRY_STATE_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all states that exist in the system. States are
     *  context for an object in the system. For instance, a player character
     *  may have a "jumping" state that is active when the character is within
     *  a jump. States can be queried on an object and changes in state can be
     *  responded to through animates, applying new states, allowing certain
     *  actions, etc.
     */
    class State {
    public:
      /*
       *  Registers or returns the existing id for the given state.
       */
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
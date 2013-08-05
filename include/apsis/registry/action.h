#ifndef APSIS_REGISTRY_ACTION_H
#define APSIS_REGISTRY_ACTION_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all actions that exist in the system. Actions
     *  are an input-triggered event. For instance, you may have an Action for
     *  "jump" that can be bound to a key. The rules in the game will respond
     *  to a held or triggered action.
     */
    class Action {
    public:
      /*
       *  Registers or returns the existing id for the given action.
       */
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
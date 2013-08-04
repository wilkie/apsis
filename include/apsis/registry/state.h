#ifndef APSIS_REGISTRY_STATE_H
#define APSIS_REGISTRY_STATE_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class State {
    public:
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
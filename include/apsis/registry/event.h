#ifndef APSIS_REGISTRY_EVENT_H
#define APSIS_REGISTRY_EVENT_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class Event {
    public:
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
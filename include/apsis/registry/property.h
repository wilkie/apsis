#ifndef APSIS_REGISTRY_PROPERTY_H
#define APSIS_REGISTRY_PROPERTY_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class Property {
    public:
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
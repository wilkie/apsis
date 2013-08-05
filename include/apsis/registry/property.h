#ifndef APSIS_REGISTRY_PROPERTY_H
#define APSIS_REGISTRY_PROPERTY_H

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all properties in the system. A property is a
     *  key of which an object can have a value. For instance, a common
     *  property for an object to have would be "hitPoints" which contains the
     *  health of the actor. Events can trigger rules that will then subtract
     *  or add to this value for an object.
     */
    class Property {
    public:
      /*
       *  Registers or returns the existing id for the given property.
       */
      static unsigned int id(const char* name);
    private:
      static std::vector<std::string> _ids;
    };
  }
}

#endif
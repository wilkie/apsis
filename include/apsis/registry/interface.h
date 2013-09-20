#ifndef APSIS_REGISTRY_INTERFACE_H
#define APSIS_REGISTRY_INTERFACE_H

#include "apsis/engine/object.h"

#include <json/json.h>

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class Interface {
    public:
      static const Registry::Interface& load(const char* path,
                                             const Engine::Object& loader);
    private:
      // Constructors
      Interface(const char* path,
                const Engine::Object& loader);

      // Keeps track of Interfaces system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Registry::Interface*> _interfaces;

      // Windows
      Apsis::Interface::Window _window;

      // Parse widget info
      void _parseWidgets(Json::Value& value,
                         Apsis::Interface::Window& parent,
                         const Engine::Object& loader);
    };
  }
}

#endif
#ifndef APSIS_REGISTRY_INTERFACE_H
#define APSIS_REGISTRY_INTERFACE_H

#include "apsis/engine/object.h"
#include "apsis/interface/window.h"

#include <json/json.h>

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class Interface {
    public:
      static const Registry::Interface& load(const char* path,
                                             const Engine::Object& loader);

      /*
       *  Returns an instance of this interface with the given size.
       */
      Apsis::Interface::Window* instance(float width, float height) const;

      /*
       *  Returns the unique global identifier for this Interface.
       */
      unsigned int id() const;
    private:
      // Constructors
      Interface(const char* path,
                const Engine::Object& loader);

      // Keeps track of Interfaces system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Registry::Interface*> _interfaces;

      // Id
      unsigned int _id;

      // Windows
      std::vector<const Apsis::Registry::Widget*> _widgets;
      std::vector<const Apsis::World::Object*>    _objects;
      std::vector<unsigned int>                   _spans;

      // Parse widget info
      void _parseWidgets(Json::Value& value,
                         const Engine::Object& loader);

      void _buildWidgets(Apsis::Interface::Window* parent,
                         unsigned int span_index,
                         unsigned int node_index,
                         unsigned int& number_spans,
                         unsigned int& number_nodes) const;
    };
  }
}

#endif
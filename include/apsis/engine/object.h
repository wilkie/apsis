#ifndef APSIS_ENGINE_OBJECT_H
#define APSIS_ENGINE_OBJECT_H

#include <json/json.h>

namespace Apsis {
  namespace Engine {
    // TODO: Document defaults when they are known
    // TODO: Document the specification for the "paths" description JSON.

    /*
     *  The object engine manages paths and loaded objects within the system
     *  and coordinates on your behalf with the object and name registry.
     *  Load and query all system objects through this interface.
     */ 
    class Object {
    public:
      /*
       *  Constructs an Object engine from the given JSON description.
       *  Should point to a file containing a "paths" type.
       */
      static Apsis::Engine::Object& load(const char* path);

      /*
       *  Constructs an Object engine from the given JSON object.
       *  Should be an object referred to by the name "paths".
       */
      static Apsis::Engine::Object& load(Json::Value& value);

      /*
       *  Constructs an Object engine that has default properties.
       */
      static Apsis::Engine::Object& default();

    private:
      // Constructors
      Object(const char* path);
      Object(Json::Value& value);
      Object();

      // Json loader
      void _loadFromJSON(Json::Value& value);

      // Default loader
      void _loadDefaults();

      std::string _scene_path;
      std::string _thing_path;
      std::string _map_path;
      std::string _binding_path;
      std::string _graphics_path;
      std::string _background_path;
      std::string _rule_path;

      // Keeps track of Object engines system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::Engine::Object*> _object_engines;
    };
  }
}

#endif
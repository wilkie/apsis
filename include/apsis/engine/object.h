#ifndef APSIS_ENGINE_OBJECT_H
#define APSIS_ENGINE_OBJECT_H

#include <json/json.h>

namespace Apsis {
  /* PROBLEM: Object engine loads objects... sometimes objects want to load other
   * objects. Therefore, there is a necessary dependency cycle.
   *
   * To get around this, we empty declare those classes, and then have to
   * include them at the end of this header file (because it is expected
   * that since we have methods that return them, that this class is
   * available and included.)
   *
   * It is annoying, but only done in this file.
   */
  namespace Registry {
    class Scene;
    class Action;
    class Rule;
    class Property;
    class State;
    class Interface;
    class Widget;
    class Shader;
    class Program;
  }

  namespace World {
    class Map;
  }

  namespace Sprite {
    class Thing;
    class Sheet;
    class Font;
  }

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
      static Apsis::Engine::Object& basic();

      /*
       *  Loads or returns the existing Thing object with the given
       *  name or path.
       */
      const Apsis::Sprite::Thing& loadThing(const char* name) const;

      /*
       *  Loads or returns the existing Map object with the given
       *  name or path.
       */
      const Apsis::World::Map& loadMap(const char* name) const;

      /*
       *  Loads or returns the existing Sheet object with the given
       *  name or path.
       */
      const Apsis::Sprite::Sheet& loadSheet(const char* name) const;

      /*
       *  Loads or returns the existing Rule object with the given
       *  name or path.
       */
      const Apsis::Registry::Rule& loadRule(const char* name) const;

      /*
       *  Loads or returns the existing Scene object with the given
       *  name or path.
       */
      const Apsis::Registry::Scene& loadScene(const char* name) const;

      /*
       *  Loads or returns the existing Interface object with the given
       *  name or path.
       */
      const Apsis::Registry::Interface& loadInterface(const char* name) const;

      /*
       *  Loads or returns the existing Widget object with the given
       *  name or path.
       */
      const Apsis::Registry::Widget& loadWidget(const char* name) const;

      /*
       *  Loads or returns the existing Shader object with the given
       *  name or path.
       */
      const Apsis::Sprite::Font& loadFont(const char* name) const;

      /*
       *  Loads or returns the existing Shader object with the given
       *  name or path.
       */
      const Apsis::Registry::Shader& loadShader(const char* name) const;

      /*
       *  Loads or returns the existing Program object with the given
       *  name or path.
       */
      const Apsis::Registry::Program& loadProgram(const char* name) const;

      /*
       *  Loads bindings.
       */
      void loadBindings(const char* name) const;

    private:
      // Constructors
      Object(const char* path);
      Object(Json::Value& value);
      Object();

      // Json loader
      void _loadFromJSON(Json::Value& value);

      // Default loader
      void _loadDefaults();

      // File finder
      std::string _findFile(const std::string& searchPath, const std::string& name) const;
      bool _fileExists(std::string& path) const;

      // Path storage
      std::string _scene_path;
      std::string _thing_path;
      std::string _map_path;
      std::string _binding_path;
      std::string _graphics_path;
      std::string _background_path;
      std::string _rule_path;
      std::string _interface_path;
      std::string _font_path;
      std::string _widget_path;
      std::string _shader_path;
      std::string _program_path;

      // Keeps track of Object engines system-wide.
      static std::vector<Apsis::Engine::Object*> _object_engines;
    };
  }
}

#endif

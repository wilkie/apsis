#ifndef APSIS_SPRITE_THING_H
#define APSIS_SPRITE_THING_H

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/animation.h"

#include "apsis/world/object.h"

#include "apsis/sync/reference_counter.h"

#include <string>
#include <vector>

#include <json/json.h>

namespace Apsis {
  namespace Sprite {
    class Thing {
    public:
      /*
       *  Constructs a Thing from the given thing file.
       */
      Thing(const char* path);

      /*
       *  Loads a Thing from the thing info represented by the given file.
       */
      static const Apsis::Sprite::Thing& load(const char* path);

      /*
       *  Retrieves an already loaded Thing of the given id.
       */
      static const Apsis::Sprite::Thing& get(unsigned int id);

      /*
       *  Returns the sprite sheet associated with this Thing.
       */
      const Apsis::Sprite::Sheet& sheet() const;

      /*
       *  Returns the animation represented by the given name.
       */
      const Apsis::Sprite::Animation& animation(const char* name) const;

      /*
       *  Returns the id of the animation represented by the given name.
       */
      unsigned int animationId(const char* name) const;

      /*
       *  Returns the animation represented by the given id.
       */
      const Apsis::Sprite::Animation& animationById(unsigned int id) const;

      /*
       *  Returns the number of animations known to this Thing.
       */
      unsigned int animationCount() const;

      /*
       *  Returns the unique id of this Thing.
       */
      unsigned int id() const;

      /*
       *  Returns the Object representation of this Thing.
       */
      const World::Object& object() const;

    private:
      // Keeps track of Things system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::Sprite::Thing> _things;

      // The path to the thing description.
      std::string _path;

      // Parses the given json via the path given in jsonFile.
      void _openJSONFile();

      // Whether or not the JSON has been loaded
      bool _jsonLoaded;

      // Load sprite sheet from JSON
      const Apsis::Sprite::Sheet& _loadSpriteSheet();

      // Parse JSON
      void _parseJSONFile();

      // JSON value;
      Json::Value _value;

      // The default object representation.
      World::Object _object;

      // Animations
      std::vector<Apsis::Sprite::Animation> _animations;

      // Sprite sheet
      const Sprite::Sheet& _sheet;

      // Thing id
      unsigned int _id;

      // For garbage collection
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif
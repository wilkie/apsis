#ifndef APSIS_WORLD_MAP_H
#define APSIS_WORLD_MAP_H

#include "apsis/engine/object.h"

#include "apsis/world/tile.h"
#include "apsis/world/camera.h"

#include "apsis/sprite/sheet.h"

#include "apsis/primitive/vertex_array.h"
#include "apsis/primitive/vertex_buffer.h"

#include "apsis/sync/reference_counter.h"

#include <json/json.h>

#include <vector>

namespace Apsis {
  namespace World {
    /*
     *  Describes a world made out of tiles drawn from a single SpriteSheet.
     */
    class Map {
    public:
      // TODO: Describe map json.
      // TODO: Allow programmable maps?

      /*
       *  Constructs a Map from the given JSON description of the map.
       */
      static const Apsis::World::Map& load(const char* json,
                                           const Apsis::Engine::Object& loader);

      /*
       *  Queries the tile at the world coordinates (x,y)
       */
      Tile& tile(unsigned int x, unsigned int y);
      const Tile& tile(unsigned int x, unsigned int y) const;

      /*
       *  Returns the width of the map.
       */
      unsigned int width() const;

      /*
       *  Returns the height of the map.
       */
      unsigned int height() const;

      /*
       *  Returns the width of a tile.
       */
      float tileWidth() const;

      /*
       *  Returns the height of a tile.
       */
      float tileHeight() const;

      /*
       *  Returns the Apsis::Sprite::Sheet being used to draw the map.
       */
      const Apsis::Sprite::Sheet& spriteSheet();

      /*
       *  Renders the map.
       */
      void draw(const Primitive::Matrix& projection,
                const World::Camera& camera) const;

      /*
       *  Returns the unique id for this map.
       */
      unsigned int id() const;

    private:
      // Keeps track of Maps system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::World::Map*> _maps;

      // Constructor
      Map(const char* json,
          const Engine::Object& loader);

      /*
       *  Constructs a Apsis::World::Map of the given width and height and
       *    drawn using the given Apsis::Sprite::Sheet.
       */
      Map(const Engine::Object& loader,
          unsigned int width,
          unsigned int height,
          float tileWidth,
          float tileHeight,
          const Apsis::Sprite::Sheet& spriteSheet);

      const Engine::Object& _loader;

      // JSON parsing

      // The path to the map description.
      std::string _path;

      // Whether or not the JSON has been loaded
      bool _jsonLoaded;

      // JSON value;
      Json::Value _value;

      // Parses the given json via the path given in jsonFile.
      void _openJSONFile();

      // Load sprite sheet from JSON
      const Apsis::Sprite::Sheet& _loadSpriteSheet();

      // Parse JSON
      void _parseJSONFile();

      // Internal storage
      unsigned int _width;
      unsigned int _height;
      const Apsis::Sprite::Sheet& _sheet;

      // The tiles that compose the map
      std::vector<Tile> _tiles;

      float _tileWidth;
      float _tileHeight;

      void _generateVAO();

      Primitive::VertexArray _vao;

      float* _vertices;
      Primitive::VertexBuffer _vbo;

      unsigned short* _elements;
      Primitive::VertexBuffer _ebo;

      // Map unique id
      unsigned int _id;
    };
  }
}

#endif

#ifndef APSIS_WORLD_MAP_H
#define APSIS_WORLD_MAP_H

#include "apsis/world/tile.h"
#include "apsis/sprite/sheet.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include "apsis/sync/reference_counter.h"

#include <glm/glm.hpp>

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
      static Apsis::World::Map& load(const char* json);

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
      void draw(const glm::mat4& projection,
                Primitives::Camera& camera,
                const glm::mat4& model) const;

      /*
       *  Returns the unique id for this map.
       */
      unsigned int id() const;

    private:
      // Keeps track of Maps system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::World::Map*> _maps;

      // Constructor
      Map(const char* json);

      /*
       *  Constructs a Apsis::World::Map of the given width and height and
       *    drawn using the given Apsis::Sprite::Sheet.
       */
      Map(unsigned int width,
          unsigned int height,
          float tileWidth,
          float tileHeight,
          const Apsis::Sprite::Sheet& spriteSheet);

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

      Primitives::VertexArray _vao;

      float* _vertices;
      Primitives::VertexBuffer _vbo;

      unsigned int* _elements;
      Primitives::VertexBuffer _ebo;

      // Map unique id
      unsigned int _id;
    };
  }
}

#endif

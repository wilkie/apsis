#include "apsis/world/map.h"

#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <fstream>

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include <algorithm>
#include <fstream>

std::vector<std::string> Apsis::World::Map::_ids;
std::vector<Apsis::World::Map*> Apsis::World::Map::_maps;

const Apsis::World::Map& Apsis::World::Map::load(const char* json,
                                                 const Apsis::Engine::Object& loader) {
  Apsis::World::Map* map = new Apsis::World::Map(json);
  _maps.push_back(map);
  return *map;
}

Apsis::World::Map::Map(const char* json)
  : _jsonLoaded(false),
    _path(json),
    _sheet(_loadSpriteSheet()) {

  _parseJSONFile();
  _generateVAO();
}

unsigned int Apsis::World::Map::id() const {
  return _id;
}

Apsis::World::Map::Map(unsigned int width,
                       unsigned int height,
                       float tileWidth,
                       float tileHeight,
                       const Apsis::Sprite::Sheet& spriteSheet) :
                   _width(width),
                   _height(height),
                   _sheet(spriteSheet),
                   _tileWidth(tileWidth),
                   _tileHeight(tileHeight) {

  // Blank out tiles
  for(unsigned int x = 0; x < _width * _height; x++) {
    Apsis::World::Tile tile = Apsis::World::Tile();
    tile.spriteIndex(0xffffffff);
    tile.passable(true);

    _tiles.push_back(tile);
  }

  _generateVAO();
}

void Apsis::World::Map::_generateVAO() {
  // width = 3, height = 2 creates map like this:
  //
  //   +---+---+---+
  //   |   |   |   |
  //   +---+---+---+
  //   |   |   |   |
  //   +---+---+---+
  //
  // which split up into drawable pieces:
  //
  //   +---+   +---+   +---+
  //   |---| x |---| x |---|
  //   +---+   +---+   +---+
  //     x       x       x
  //   +---+   +---+   +---+  (24 vertices: 4 * width * height)
  //   |---| x |---| x |---|
  //   +---+   +---+   +---+  (2wh-w-h quads between tiles)

  // Create buffer array:
  unsigned int vertices_size = 4 * _width * _height;
  unsigned int elements_size = 6 * _width * _height;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   2 per texcoord
  this->_vertices = new float[5 * vertices_size];

  unsigned int i = 0;
  unsigned int ei = 0;
  unsigned int ti = 0;

  for (unsigned int h = 0; h < _height; h++) {
    for (unsigned int w = 0; w < _width; w++) {
      if (_tiles[ti].spriteIndex() == 0xffffffff) {
        vertices_size -= 4;
        elements_size -= 6;
        ti++;
        continue;
      }

      unsigned int si = _tiles[ti].spriteIndex();

      float coords[4];
      _sheet.textureCoordinates(si, coords);

      float spriteWidth  = _sheet.width(si);
      float spriteHeight = _sheet.height(si);

      ti++;

      _vertices[i * 5 + 0] = (float)w*_tileWidth;
      _vertices[i * 5 + 1] = 0.0f;
      _vertices[i * 5 + 2] = (float)h*_tileHeight + _tileHeight - spriteHeight;

      _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
      _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

      i++;

      _vertices[i * 5 + 0] = (float)w*_tileWidth + spriteWidth;
      _vertices[i * 5 + 1] = 0.0f;
      _vertices[i * 5 + 2] = (float)h*_tileHeight + _tileHeight - spriteHeight;

      _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
      _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

      i++;

      _vertices[i * 5 + 0] = (float)w*_tileWidth + spriteWidth;
      _vertices[i * 5 + 1] = 0.0f;
      _vertices[i * 5 + 2] = (float)h*_tileHeight + _tileHeight;

      _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
      _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

      i++;

      _vertices[i * 5 + 0] = (float)w*_tileWidth;
      _vertices[i * 5 + 1] = 0.0f;
      _vertices[i * 5 + 2] = (float)h*_tileHeight + _tileHeight;

      _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
      _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

      i++;

      _elements[ei] = i-4; ei++;
      _elements[ei] = i-3; ei++;
      _elements[ei] = i-1; ei++;

      _elements[ei] = i-3; ei++;
      _elements[ei] = i-2; ei++;
      _elements[ei] = i-1; ei++;
    }
  }

  _vbo.transfer(_vertices, 5 * vertices_size);
  _ebo.transfer(_elements, elements_size);

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("src/shaders/fragment/flat.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  program.defineInput("position", _vbo, 3, Primitives::Program::Type::Float, false, 5, 0);
  program.defineInput("texcoord", _vbo, 2, Primitives::Program::Type::Float, false, 5, 3);

  _vao.defineUniform("model", program);
  _vao.defineUniform("view",  program);
  _vao.defineUniform("proj",  program);

  _vao.defineUniform("tex", program);
  _vao.bindTexture(0, *_sheet.texture());
  _vao.uploadUniform("tex", 0);
}

unsigned int Apsis::World::Map::width() const {
  return _width;
}

unsigned int Apsis::World::Map::height() const {
  return _height;
}

Apsis::World::Tile& Apsis::World::Map::tile(unsigned int x, unsigned int y) {
  return _tiles[(y * _width) + x];
}

const Apsis::World::Tile& Apsis::World::Map::tile(unsigned int x, unsigned int y) const {
  return _tiles[(y * _width) + x];
}

const Apsis::Sprite::Sheet& Apsis::World::Map::spriteSheet() {
  return _sheet;
}

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

void Apsis::World::Map::draw(const glm::mat4& projection,
                             Primitives::Camera& camera,
                             const glm::mat4& model) const {
  const float (*matrix)[4] = (const float (*)[4])glm::value_ptr(projection);
  _vao.uploadUniform("proj", matrix);
  const float (*view_matrix)[4] = (const float (*)[4])glm::value_ptr(camera.view());
  _vao.uploadUniform("view", view_matrix);
  const float (*model_matrix)[4] = (const float (*)[4])glm::value_ptr(model);
  _vao.uploadUniform("model", model_matrix);

  _vao.uploadUniform("camera", camera.eye());
  _vao.draw();
}

float Apsis::World::Map::tileWidth() const {
  return _tileWidth;
}

float Apsis::World::Map::tileHeight() const {
  return _tileHeight;
}

void Apsis::World::Map::_openJSONFile() {
  if (_jsonLoaded) {
    return;
  }

  Json::Reader reader;

  std::ifstream file(_path);
  reader.parse(file, _value);
  file.close();

  _jsonLoaded = true;
}

const Apsis::Sprite::Sheet& Apsis::World::Map::_loadSpriteSheet() {
  _openJSONFile();

  return Apsis::Sprite::Sheet::load(_value["sprites"].asCString());
}

void Apsis::World::Map::_parseJSONFile() {
  _openJSONFile();

  _width  = _value["width"].asUInt();
  _height = _value["height"].asUInt();

  _tileWidth  = (float)_value["tile_width"].asUInt();
  _tileHeight = (float)_value["tile_height"].asUInt();

  for (Json::Value::iterator it_y = _value["tiles"].begin(); it_y != _value["tiles"].end(); ++it_y) {
    for (Json::Value::iterator it_x = (*it_y).begin(); it_x != (*it_y).end(); ++it_x) {
      Apsis::World::Tile tile = Apsis::World::Tile();
      int tileIndex = (*it_x).asInt();
      if (tileIndex >= 0) {
        tile.spriteIndex(_value["tile_descriptions"][tileIndex]["index"].asUInt());
        tile.passable(_value["tile_descriptions"][tileIndex]["passable"].asBool());
      }
      else {
        tile.spriteIndex(0xffffffff);
        tile.passable(true);
      }

      _tiles.push_back(tile);
    }
  }
}
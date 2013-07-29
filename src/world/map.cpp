#include "apsis/world/map.h"

#include <stdlib.h>
#include <string.h>

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

Apsis::World::Map::Map(unsigned int width,
                       unsigned int height,
                       float tileWidth,
                       float tileHeight,
                       Apsis::Primitives::SpriteSheet* spriteSheet) :
                   _width(width),
                   _height(height),
                   _tileWidth(tileWidth),
                   _tileHeight(tileHeight) {
  for(unsigned int x = 0; x < _width * _height; x++) {
    Apsis::World::Tile tile = Apsis::World::Tile();
    tile.spriteIndex(rand() % 16);
    tile.passable(true);

    _tiles.push_back(tile);
  }

  _generateWalls();

  _spriteSheet = spriteSheet;

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
  unsigned int vertices_size = 4 * width * height;
  unsigned int elements_size = 6 * width * height;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   2 per texcoord
  this->_vertices = new float[5 * vertices_size];
  
  unsigned int i = 0;
  unsigned int ei = 0;
  unsigned int ti = 0;

  for (unsigned int h = 0; h < height; h++) {
    for (unsigned int w = 0; w < width; w++) {
      if (_tiles[ti].spriteIndex() == 0xffff) {
        vertices_size -= 4;
        elements_size -= 6;
        ti++;
        continue;
      }

      float coords[4];
      _spriteSheet->textureCoordinates(_tiles[ti].spriteIndex(), coords);
      Apsis::Primitives::Sprite* sprite = _spriteSheet->sprite(_tiles[ti].spriteIndex());
      ti++;

      _vertices[i * 5 + 0] = (float)w*_tileWidth;
      _vertices[i * 5 + 1] = 0.0f;
      _vertices[i * 5 + 2] = (float)h*_tileHeight + _tileHeight - sprite->height;

      _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
      _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

      i++;

      _vertices[i * 5 + 0] = (float)w*_tileWidth + sprite->width;
      _vertices[i * 5 + 1] = 0.0f;
      _vertices[i * 5 + 2] = (float)h*_tileHeight + _tileHeight - sprite->height;

      _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
      _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

      i++;
      
      _vertices[i * 5 + 0] = (float)w*_tileWidth + sprite->width;
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
  _vao.bindTexture(0, *_spriteSheet->texture());
  _vao.uploadUniform("tex", 0);
}

unsigned int Apsis::World::Map::width() {
  return _width;
}

unsigned int Apsis::World::Map::height() {
  return _height;
}

Apsis::World::Tile* Apsis::World::Map::tile(unsigned int x, unsigned int y) {
  return &_tiles[(y * _width) + x];
}

Apsis::Primitives::SpriteSheet* Apsis::World::Map::spriteSheet() {
  return _spriteSheet;
}

void Apsis::World::Map::_generateWalls() {
  for(unsigned int x = 0; x < _width * _height; x++) {
    _tiles[x].spriteIndex(0xffff);
    _tiles[x].passable(true);
  }

  _tiles[2 + _width*8].spriteIndex(81);
  _tiles[2 + _width*8].passable(false);
  _tiles[3 + _width*8].spriteIndex(82);
  _tiles[3 + _width*8].passable(false);
  _tiles[4 + _width*8].spriteIndex(83);
  _tiles[4 + _width*8].passable(false);

  _tiles[6 + _width*10].spriteIndex(81);
  _tiles[6 + _width*10].passable(false);
  _tiles[7 + _width*10].spriteIndex(82);
  _tiles[7 + _width*10].passable(false);
  _tiles[8 + _width*10].spriteIndex(83);
  _tiles[8 + _width*10].passable(false);

  _tiles[12 + _width*8].spriteIndex(81);
  _tiles[12 + _width*8].passable(false);
  _tiles[13 + _width*8].spriteIndex(82);
  _tiles[13 + _width*8].passable(false);
  _tiles[14 + _width*8].spriteIndex(83);
  _tiles[14 + _width*8].passable(false);

  _tiles[18 + _width*5].spriteIndex(81);
  _tiles[18 + _width*5].passable(false);
  _tiles[19 + _width*5].spriteIndex(82);
  _tiles[19 + _width*5].passable(false);
  _tiles[20 + _width*5].spriteIndex(83);
  _tiles[20 + _width*5].passable(false);

  _tiles[20 + _width*4].spriteIndex(1);
  _tiles[20 + _width*4].passable(false);
  _tiles[19 + _width*4].spriteIndex(0);
  _tiles[19 + _width*4].passable(false);
  _tiles[20 + _width*3].spriteIndex(0);
  _tiles[20 + _width*3].passable(false);

  _tiles[12 + _width*5].spriteIndex(2);
  _tiles[12 + _width*5].passable(false);
  _tiles[14 + _width*5].spriteIndex(2);
  _tiles[14 + _width*5].passable(false);

  _tiles[13 + _width*7].spriteIndex(128);

  _tiles[16 + _width*10].spriteIndex(67);
  _tiles[16 + _width*10].passable(false);
  _tiles[17 + _width*10].spriteIndex(82);
  _tiles[17 + _width*10].passable(false);
  _tiles[18 + _width*10].spriteIndex(82);
  _tiles[18 + _width*10].passable(false);

  _tiles[25 + _width*10].spriteIndex(67);
  _tiles[25 + _width*10].passable(false);
  _tiles[26 + _width*10].spriteIndex(82);
  _tiles[26 + _width*10].passable(false);
  _tiles[27 + _width*10].spriteIndex(82);
  _tiles[27 + _width*10].passable(false);

  _tiles[18 + _width*9].spriteIndex(172);
  _tiles[18 + _width*11].passable(false);
  _tiles[19 + _width*10].spriteIndex(16);
  _tiles[19 + _width*10].passable(false);
  _tiles[20 + _width*10].spriteIndex(16);
  _tiles[20 + _width*10].passable(false);
  _tiles[21 + _width*10].spriteIndex(16);
  _tiles[21 + _width*10].passable(false);
  _tiles[22 + _width*10].spriteIndex(16);
  _tiles[22 + _width*10].passable(false);
  
  _tiles[18 + _width*11].spriteIndex(65);
  _tiles[18 + _width*11].passable(false);
  _tiles[18 + _width*12].spriteIndex(65);
  _tiles[18 + _width*12].passable(false);
  _tiles[18 + _width*13].spriteIndex(65);
  _tiles[18 + _width*13].passable(false);
  _tiles[18 + _width*14].spriteIndex(65);
  _tiles[18 + _width*14].passable(false);
  _tiles[18 + _width*15].spriteIndex(65);
  _tiles[18 + _width*15].passable(false);
  
  _tiles[17 + _width*11].spriteIndex(65);
  _tiles[17 + _width*11].passable(false);
  _tiles[17 + _width*12].spriteIndex(65);
  _tiles[17 + _width*12].passable(false);
  _tiles[17 + _width*13].spriteIndex(65);
  _tiles[17 + _width*13].passable(false);
  _tiles[17 + _width*14].spriteIndex(65);
  _tiles[17 + _width*14].passable(false);
  _tiles[17 + _width*15].spriteIndex(65);
  _tiles[17 + _width*15].passable(false);

  _tiles[11 + _width*11].spriteIndex(82);
  _tiles[11 + _width*11].passable(false);
  _tiles[10 + _width*11].spriteIndex(67);
  _tiles[10 + _width*11].passable(false);

  _tiles[16 + _width*12].spriteIndex(173);
  _tiles[16 + _width*12].passable(false);
  _tiles[15 + _width*12].spriteIndex(173);
  _tiles[15 + _width*12].passable(false);
  _tiles[14 + _width*12].spriteIndex(173);
  _tiles[14 + _width*12].passable(false);
  _tiles[13 + _width*12].spriteIndex(173);
  _tiles[13 + _width*12].passable(false);
  _tiles[12 + _width*12].spriteIndex(173);
  _tiles[12 + _width*12].passable(false);
  _tiles[11 + _width*12].spriteIndex(65);
  _tiles[11 + _width*12].passable(false);

  _tiles[16 + _width*13].spriteIndex(65);
  _tiles[16 + _width*13].passable(false);
  _tiles[15 + _width*13].spriteIndex(65);
  _tiles[15 + _width*13].passable(false);
  _tiles[14 + _width*13].spriteIndex(65);
  _tiles[14 + _width*13].passable(false);
  _tiles[13 + _width*13].spriteIndex(65);
  _tiles[13 + _width*13].passable(false);
  _tiles[12 + _width*13].spriteIndex(65);
  _tiles[12 + _width*13].passable(false);
  _tiles[11 + _width*13].spriteIndex(65);
  _tiles[11 + _width*13].passable(false);

  _tiles[16 + _width*14].spriteIndex(65);
  _tiles[16 + _width*14].passable(false);
  _tiles[15 + _width*14].spriteIndex(65);
  _tiles[15 + _width*14].passable(false);
  _tiles[14 + _width*14].spriteIndex(65);
  _tiles[14 + _width*14].passable(false);
  _tiles[13 + _width*14].spriteIndex(65);
  _tiles[13 + _width*14].passable(false);
  _tiles[12 + _width*14].spriteIndex(65);
  _tiles[12 + _width*14].passable(false);
  _tiles[11 + _width*14].spriteIndex(65);
  _tiles[11 + _width*14].passable(false);

  _tiles[19 + _width*12].spriteIndex(95);
  _tiles[19 + _width*12].passable(false);
  _tiles[20 + _width*12].spriteIndex(95);
  _tiles[20 + _width*12].passable(false);
  _tiles[21 + _width*12].spriteIndex(95);
  _tiles[21 + _width*12].passable(false);
  _tiles[22 + _width*12].spriteIndex(95);
  _tiles[22 + _width*12].passable(false);
  _tiles[23 + _width*12].spriteIndex(95);
  _tiles[23 + _width*12].passable(false);
  _tiles[24 + _width*12].spriteIndex(95);
  _tiles[24 + _width*12].passable(false);
  _tiles[25 + _width*12].spriteIndex(95);
  _tiles[25 + _width*12].passable(false);
  _tiles[19 + _width*13].spriteIndex(93);
  _tiles[19 + _width*13].passable(false);
  _tiles[20 + _width*13].spriteIndex(93);
  _tiles[20 + _width*13].passable(false);
  _tiles[21 + _width*13].spriteIndex(93);
  _tiles[21 + _width*13].passable(false);
  _tiles[22 + _width*13].spriteIndex(93);
  _tiles[22 + _width*13].passable(false);
  _tiles[23 + _width*13].spriteIndex(93);
  _tiles[23 + _width*13].passable(false);
  _tiles[24 + _width*13].spriteIndex(93);
  _tiles[24 + _width*13].passable(false);
  _tiles[25 + _width*13].spriteIndex(93);
  _tiles[25 + _width*13].passable(false);
  _tiles[19 + _width*14].spriteIndex(93);
  _tiles[19 + _width*14].passable(false);
  _tiles[20 + _width*14].spriteIndex(93);
  _tiles[20 + _width*14].passable(false);
  _tiles[21 + _width*14].spriteIndex(93);
  _tiles[21 + _width*14].passable(false);
  _tiles[22 + _width*14].spriteIndex(93);
  _tiles[22 + _width*14].passable(false);
  _tiles[23 + _width*14].spriteIndex(93);
  _tiles[23 + _width*14].passable(false);
  _tiles[24 + _width*14].spriteIndex(93);
  _tiles[24 + _width*14].passable(false);
  _tiles[25 + _width*14].spriteIndex(93);
  _tiles[25 + _width*14].passable(false);
  _tiles[19 + _width*15].spriteIndex(93);
  _tiles[19 + _width*15].passable(false);
  _tiles[20 + _width*15].spriteIndex(93);
  _tiles[20 + _width*15].passable(false);
  _tiles[21 + _width*15].spriteIndex(93);
  _tiles[21 + _width*15].passable(false);
  _tiles[22 + _width*15].spriteIndex(93);
  _tiles[22 + _width*15].passable(false);
  _tiles[23 + _width*15].spriteIndex(93);
  _tiles[23 + _width*15].passable(false);
  _tiles[24 + _width*15].spriteIndex(93);
  _tiles[24 + _width*15].passable(false);
  _tiles[25 + _width*15].spriteIndex(93);
  _tiles[25 + _width*15].passable(false);
  _tiles[25 + _width*15].spriteIndex(93);
  _tiles[25 + _width*15].passable(false);
}

void Apsis::World::Map::draw(glm::mat4& projection,
                             Primitives::Camera& camera,
                             glm::mat4& model) {
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.uploadUniform("model", model);
  
  _vao.bindTexture(0, *_spriteSheet->texture());
  _vao.uploadUniform("camera", camera.eye());
  _vao.draw();
}

float Apsis::World::Map::tileWidth() {
  return _tileWidth;
}

float Apsis::World::Map::tileHeight() {
  return _tileHeight;
}
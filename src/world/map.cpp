#include "apsis/world/map.h"

#include <stdlib.h>
#include <string.h>

Apsis::World::Map::Map(unsigned int width,
                       unsigned int height,
                       Apsis::Primitives::SpriteSheet* spriteSheet) :
                   _width(width),
                   _height(height) {
  for(unsigned int x = 0; x < _width * _height; x++) {
    Apsis::World::Tile tile = Apsis::World::Tile();
    tile.spriteIndex(rand() % 16);
    tile.passable(true);

    _tiles.push_back(tile);
  }

  _generateWalls();

  //_tiles[9 + 6 * _width].passable = false;
  //_tiles[7 + 6 * _width].passable = false;
  //_tiles[8 + 4 * _width].passable = false;

  _spriteSheet = spriteSheet;
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
    if(rand() % 5 == 1) {
      _tiles[x].spriteIndex(16);
      _tiles[x].passable(false);
    }
  }
}

void Apsis::World::Map::draw(Apsis::Renderer& renderer) {
  renderer.bindTexture(_spriteSheet->texture());
  
  double coords[4];

  // Render all background tiles
  //renderer.depthTest(false);
  for (long y = this->height() - 1; y >= 0; y--) {
    for (long x = 0; x < (long)this->width(); x++) {
      Tile* tile = this->tile(x, y);
      if (!tile->passable()) {
        continue;
      }

      this->spriteSheet()->textureCoordinates(tile->spriteIndex(), coords);

      Apsis::Primitives::Sprite* sprite = this->spriteSheet()->sprite(tile->spriteIndex());
      /*renderer.drawSquare(x * 32.0, y * 32.0,
                          sprite->width, sprite->height,
                          coords[0], coords[1], coords[2], coords[3],
                          0);*/
    }
  }
}
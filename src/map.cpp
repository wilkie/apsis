#include "iso-tasty/map.h"

#include <cstddef>
#include <string.h>
#include <math.h>

#include <stdio.h>
#include <Windows.h>

Apsis::Map::Map(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _x(width/2), 
  _z(height/2) {
  _map = new World::Tile*[width * height];
  for (unsigned int x = 0; x < width; x++) {
    for (unsigned int z = 0; z < height; z++) {
      unsigned int offset = (_width * x + z) * 44;
      _map[_width * x + z] = new World::Tile();
    }
  }
}

Apsis::Map::~Map() {
  delete [] _map;
}

unsigned int Apsis::Map::width() {
  return _width;
}

unsigned int Apsis::Map::height() {
  return _height;
}

Apsis::World::Tile* Apsis::Map::atIndex(unsigned int x, unsigned int z) {
  return _map[_width * x + z];
}

unsigned int Apsis::Map::x() {
  return _x;
}

unsigned int Apsis::Map::x(unsigned int value) {
  _x = value;
  if (_x >= _width) {
    _x = _width-1;
  }
  return _x;
}

unsigned int Apsis::Map::z() {
  return _z;
}

unsigned int Apsis::Map::z(unsigned int value) {
  _z = value;
  if (_z >= _height) {
    _z = _height-1;
  }
  return _z;
}

void Apsis::Map::raise(float amount) {
  // Every tile this touches is raised a little bit
  Apsis::World::Tile* tile;
  float old_height;

  if (_x != _width && _z != _height) {
    // Bottom Right
    tile = atIndex(_x, _z);
    old_height = tile->cornerHeight(Apsis::TOP_LEFT);
    tile->cornerHeight(Apsis::TOP_LEFT, old_height + amount);
  }

  if (_x != 0 && _z != _height) {
    // Bottom Left
    tile = atIndex(_x-1, _z);
    old_height = tile->cornerHeight(Apsis::TOP_RIGHT);
    tile->cornerHeight(Apsis::TOP_RIGHT, old_height + amount);
  }

  if (_x != _width && _z != 0) {
    // Top Right
    tile = atIndex(_x, _z-1);
    old_height = tile->cornerHeight(Apsis::BOT_LEFT);
    tile->cornerHeight(Apsis::BOT_LEFT, old_height + amount);
  }

  if (_x != 0 && _z != 0) {
    // Top Left
    tile = atIndex(_x-1, _z-1);
    old_height = tile->cornerHeight(Apsis::BOT_RIGHT);
    tile->cornerHeight(Apsis::BOT_RIGHT, old_height + amount);
  }

  // Regenerate curve points along this point

  // Horizontal

  Apsis::Geometry::Point3d* line;
  Apsis::Geometry::Point3d* first;
  Apsis::Geometry::Point3d* second;
  line = new Apsis::Geometry::Point3d[_width+1];
  first = new Apsis::Geometry::Point3d[_width];
  second = new Apsis::Geometry::Point3d[_width];

  for (unsigned int x = 0; x < _width; x++) {
    tile = atIndex(x, _z);
    line[x].x = (float)x;
    line[x].y = tile->cornerHeight(Apsis::TOP_LEFT);
    line[x].z = 0.0f; // does not matter
  }
  line[_width].x = (float)_width;
  line[_width].y = tile->cornerHeight(Apsis::TOP_RIGHT);
  line[_width].z = 0.0f;

  controlPoints(line, first, second, _width+1);

  // Pass control points back to tiles

  for (unsigned int x = 0; x < _width; x++) {
    tile = atIndex(x, _z);
    tile->firstControl(Apsis::TOP_LEFT, first[x].y);
    tile->secondControl(Apsis::TOP_LEFT, second[x].y);
    if (_z != 0) {
      tile = atIndex(x, _z-1);
      tile->firstControl(Apsis::BOT_RIGHT, first[x].y);
      tile->secondControl(Apsis::BOT_RIGHT, second[x].y);
    }
  }

  delete [] first;
  delete [] second;
  delete [] line;

  // Vertical

  line = new Point[_height+1];
  first = new Point[_height+1];
  second = new Point[_height+1];

  for (unsigned int z = 0; z < _height; z++) {
    tile = atIndex(_x, z);
    line[z].x = (float)z; // does not matter
    line[z].y = tile->cornerHeight(Apsis::TOP_LEFT);
    line[z].z = 0.0f;
  }
  line[_height].x = (float)_height;
  line[_height].y = tile->cornerHeight(Apsis::BOT_LEFT);
  line[_height].z = 0.0f;

  controlPoints(line, first, second, _height+1);

  // Pass control points back to tiles

  for (unsigned int z = 0; z < _height; z++) {
    tile = atIndex(_x, z);
    tile->firstControl(Apsis::BOT_LEFT, first[z].y);
    tile->secondControl(Apsis::BOT_LEFT, second[z].y);
    if (_x != 0) {
      tile = atIndex(_x-1, z);
      tile->firstControl(Apsis::TOP_RIGHT, first[z].y);
      tile->secondControl(Apsis::TOP_RIGHT, second[z].y);
    }
  }

  delete [] first;
  delete [] second;
  delete [] line;
}

void Apsis::Map::lift(float amount) {
  // Just the one tile is raised at all corners
  Apsis::Tile* tile;

  tile = atIndex(_x, _z);
  for (unsigned int i = 0; i < 4; i++) {
    tile->cornerHeight(i, tile->cornerHeight(i) + amount);
    tile->firstControl(i, tile->firstControl(i) + amount);
    tile->secondControl(i, tile->secondControl(i) + amount);
  }
}

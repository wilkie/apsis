#include "iso-tasty/map.h"

#include <cstddef>
#include <string.h>

IsoTasty::Map::Map(unsigned int width, unsigned int height) :
  _width(width),
  _height(height) {
	_map = new Tile[width * height];
}

IsoTasty::Map::~Map() {
	delete [] _map;
}

unsigned int IsoTasty::Map::width() {
	return _width;
}

unsigned int IsoTasty::Map::height() {
	return _height;
}

IsoTasty::Tile* IsoTasty::Map::atIndex(unsigned int x, unsigned int z) {
	return &_map[_width * x + z];
}
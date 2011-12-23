#include "iso-tasty/map.h"

IsoTasty::Map::Map(unsigned int width, unsigned int height) :
  _width(width),
  _height(height) {
}

unsigned int IsoTasty::Map::width() {
	return _width;
}

unsigned int IsoTasty::Map::height() {
	return _height;
}
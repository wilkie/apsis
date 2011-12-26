#include "iso-tasty/map.h"

#include <cstddef>
#include <string.h>
#include <math.h>

IsoTasty::Map::Map(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _x(width/2), 
  _z(height/2) {
	_map = new Tile*[width * height];
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int z = 0; z < height; z++) {
			unsigned int offset = (_width * x + z) * 44;
			_map[_width * x + z] = new Tile();
		}
	}
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
	return _map[_width * x + z];
}

unsigned int IsoTasty::Map::x() {
	return _x;
}

unsigned int IsoTasty::Map::x(unsigned int value) {
	_x = value;
	if (_x >= _width) {
		_x = _width-1;
	}
	return _x;
}

unsigned int IsoTasty::Map::z() {
	return _z;
}

unsigned int IsoTasty::Map::z(unsigned int value) {
	_z = value;
	if (_z >= _height) {
		_z = _height-1;
	}
	return _z;
}

void IsoTasty::Map::raise(float amount) {
	// Every tile this touches is raised a little bit
	IsoTasty::Tile* tile;
	float old_height;

	if (_x != _width && _z != _height) {
		// Bottom Right
		tile = atIndex(_x, _z);
		old_height = tile->cornerHeight(IsoTasty::TOP_LEFT);
		tile->cornerHeight(IsoTasty::TOP_LEFT, old_height + amount);
	}

	if (_x != 0 && _z != _height) {
		// Bottom Left
		tile = atIndex(_x-1, _z);
		old_height = tile->cornerHeight(IsoTasty::TOP_RIGHT);
		tile->cornerHeight(IsoTasty::TOP_RIGHT, old_height + amount);
	}

	if (_x != _width && _z != 0) {
		// Top Right
		tile = atIndex(_x, _z-1);
		old_height = tile->cornerHeight(IsoTasty::BOT_LEFT);
		tile->cornerHeight(IsoTasty::BOT_LEFT, old_height + amount);
	}

	if (_x != 0 && _z != 0) {
		// Top Left
		tile = atIndex(_x-1, _z-1);
		old_height = tile->cornerHeight(IsoTasty::BOT_RIGHT);
		tile->cornerHeight(IsoTasty::BOT_RIGHT, old_height + amount);
	}
}

void IsoTasty::Map::lift(float amount) {
	// Just the one tile is raised at all corners
	IsoTasty::Tile* tile;

	tile = atIndex(_x, _z);
	for (unsigned int i = 0; i < 4; i++) {
		tile->cornerHeight(i, tile->cornerHeight(i) + amount);
	}
}
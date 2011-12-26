#include "iso-tasty/tile.h"

IsoTasty::Tile::Tile() :
  _hover(0.0f) {
	for (int i = 0; i < 4; i++) {
		_cornerHeight[i] = 5.0f;
	}
}

float IsoTasty::Tile::minHeight() {
	float ret = _cornerHeight[0];
	for (unsigned int i = 1; i < 4; i++) {
		if (_cornerHeight[i] < ret) {
			ret = _cornerHeight[i];
		}
	}
	return ret;
}

float IsoTasty::Tile::maxHeight() {
	float ret = _cornerHeight[0];
	for (unsigned int i = 1; i < 4; i++) {
		if (_cornerHeight[i] > ret) {
			ret = _cornerHeight[i];
		}
	}
	return ret;
}

float IsoTasty::Tile::hover() {
	return _hover;
}

float IsoTasty::Tile::hover(float value) {
	_hover = value;
	return _hover;
}

float IsoTasty::Tile::cornerHeight(unsigned int index) {
	if (index < 4) {
		return _cornerHeight[index];
	}
	return 0.0f;
}

float IsoTasty::Tile::cornerHeight(unsigned int index, float value) {
	if (index < 4) {
		_cornerHeight[index] = value;
		return value;
	}
	return 0.0f;
}
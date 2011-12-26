#include "iso-tasty/tile.h"

IsoTasty::Tile::Tile() :
  _hover(0.0f) {
	for (int i = 0; i < 4; i++) {
		_cornerHeight[i] = 5.0f;
		_firstControl[i] = 5.0f;
		_secondControl[i] = 5.0f;
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
		//_firstControl[index] = value;
		//_secondControl[index] = value;
		return value;
	}
	return 0.0f;
}

float IsoTasty::Tile::firstControl(unsigned int index) {
	if (index < 4) {
		return _firstControl[index];
	}
	return 0.0f;
}

float IsoTasty::Tile::firstControl(unsigned int index, float value) {
	if (index < 4) {
		_firstControl[index] = value;
		return value;
	}
	return 0.0f;
}

float IsoTasty::Tile::secondControl(unsigned int index) {
	if (index < 4) {
		return _secondControl[index];
	}
	return 0.0f;
}

float IsoTasty::Tile::secondControl(unsigned int index, float value) {
	if (index < 4) {
		_secondControl[index] = value;
		return value;
	}
	return 0.0f;
}
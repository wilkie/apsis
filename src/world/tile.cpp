#include "apsis/world/tile.h"

Apsis::World::Tile::Tile() :
  _hover(0.0f) {
  for (int i = 0; i < 4; i++) {
    _cornerHeight[i] = 5.0f;
    _firstControl[i] = 5.0f;
    _secondControl[i] = 5.0f;
  }
}

Apsis::World::Tile::Tile(float cornerHeight[4]) :
  _hover(0.0f) {
  for (int i = 0; i < 4; i++) {
    _cornerHeight[i] = 5.0f;
    _firstControl[i] = 5.0f;
    _secondControl[i] = 5.0f;
  }
}

float Apsis::World::Tile::minHeight() const {
  float ret = _cornerHeight[0];
  for (unsigned int i = 1; i < 4; i++) {
    if (_cornerHeight[i] < ret) {
      ret = _cornerHeight[i];
    }
  }
  return ret;
}

// Min height of the curve indicated by the index
float Apsis::World::Tile::minHeight(unsigned int index) {
  float start = _cornerHeight[index];
  float end = _cornerHeight[(index+1)%4];
  if (start < end) {
    return start;
  }
  return end;
}

float Apsis::World::Tile::maxHeight() const {
  float ret = _cornerHeight[0];
  for (unsigned int i = 1; i < 4; i++) {
    if (_cornerHeight[i] > ret) {
      ret = _cornerHeight[i];
    }
  }
  return ret;
}

// Max height of the curve indicated by the index
float Apsis::World::Tile::maxHeight(unsigned int index) {
  float start = _cornerHeight[index];
  float end = _cornerHeight[(index+1)%4];
  if (start > end) {
    return start;
  }
  return end;
}

float Apsis::World::Tile::hover() const {
  return _hover;
}

float Apsis::World::Tile::hover(float value) {
  _hover = value;
  return _hover;
}

float Apsis::World::Tile::cornerHeight(unsigned int index) const {
  if (index < 4) {
    return _cornerHeight[index];
  }
  return 0.0f;
}

float Apsis::World::Tile::cornerHeight(unsigned int index, float value) {
  if (index < 4) {
    _cornerHeight[index] = value;
    return value;
  }
  return 0.0f;
}

float Apsis::World::Tile::firstControl(unsigned int index) const {
  if (index < 4) {
    return _firstControl[index];
  }
  return 0.0f;
}

float Apsis::World::Tile::firstControl(unsigned int index, float value) {
  if (value < minHeight(index)) {
    value = minHeight(index);
  }
  if (value > maxHeight(index)) {
    value = maxHeight(index);
  }
  if (index < 4) {
    _firstControl[index] = value;
    return value;
  }
  return 0.0f;
}

float Apsis::World::Tile::secondControl(unsigned int index) const {
  if (index < 4) {
    return _secondControl[index];
  }
  return 0.0f;
}

float Apsis::World::Tile::secondControl(unsigned int index, float value) {
  if (value < minHeight(index)) {
    value = minHeight(index);
  }
  if (value > maxHeight(index)) {
    value = maxHeight(index);
  }
  if (index < 4) {
    _secondControl[index] = value;
    return value;
  }
  return 0.0f;
}

bool Apsis::World::Tile::passable() const {
  return _passable;
}

bool Apsis::World::Tile::passable(bool value) {
  _passable = value;
  return _passable;
}

unsigned int Apsis::World::Tile::spriteIndex() const {
  return _spriteIndex;
}

unsigned int Apsis::World::Tile::spriteIndex(unsigned int value) {
  _spriteIndex = value;
  return _spriteIndex;
}
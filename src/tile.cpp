#include "iso-tasty/tile.h"

Apsis::Tile::Tile() :
  _hover(0.0f) {
  for (int i = 0; i < 4; i++) {
    _cornerHeight[i] = 5.0f;
    _firstControl[i] = 5.0f;
    _secondControl[i] = 5.0f;
  }
}

float Apsis::Tile::minHeight() {
  float ret = _cornerHeight[0];
  for (unsigned int i = 1; i < 4; i++) {
    if (_cornerHeight[i] < ret) {
      ret = _cornerHeight[i];
    }
  }
  return ret;
}

// Min height of the curve indicated by the index
float Apsis::Tile::minHeight(unsigned int index) {
  float start = _cornerHeight[index];
  float end = _cornerHeight[(index+1)%4];
  if (start < end) {
    return start;
  }
  return end;
}

float Apsis::Tile::maxHeight() {
  float ret = _cornerHeight[0];
  for (unsigned int i = 1; i < 4; i++) {
    if (_cornerHeight[i] > ret) {
      ret = _cornerHeight[i];
    }
  }
  return ret;
}

// Max height of the curve indicated by the index
float Apsis::Tile::maxHeight(unsigned int index) {
  float start = _cornerHeight[index];
  float end = _cornerHeight[(index+1)%4];
  if (start > end) {
    return start;
  }
  return end;
}

float Apsis::Tile::hover() {
  return _hover;
}

float Apsis::Tile::hover(float value) {
  _hover = value;
  return _hover;
}

float Apsis::Tile::cornerHeight(unsigned int index) {
  if (index < 4) {
    return _cornerHeight[index];
  }
  return 0.0f;
}

float Apsis::Tile::cornerHeight(unsigned int index, float value) {
  if (index < 4) {
    _cornerHeight[index] = value;
    return value;
  }
  return 0.0f;
}

float Apsis::Tile::firstControl(unsigned int index) {
  if (index < 4) {
    return _firstControl[index];
  }
  return 0.0f;
}

float Apsis::Tile::firstControl(unsigned int index, float value) {
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

float Apsis::Tile::secondControl(unsigned int index) {
  if (index < 4) {
    return _secondControl[index];
  }
  return 0.0f;
}

float Apsis::Tile::secondControl(unsigned int index, float value) {
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
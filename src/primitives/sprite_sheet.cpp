#include "apsis/primitives/sprite_sheet.h"

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include <string.h>

#include <math.h>
#include <SOIL.h>

#include <fstream>
#include <json/json.h>

Apsis::Primitives::SpriteSheet::~SpriteSheet() {
  for (unsigned int i = 0; i < _sprites.size(); i++) {
    delete _sprites[i];
  }
}

Apsis::Primitives::SpriteSheet::SpriteSheet(const char* filename) {
  _texture = new Apsis::Primitives::Texture(filename);
  _width = _texture->width();
  _height = _texture->height();

  // Set GL texture options
  glBindTexture(GL_TEXTURE_2D, _texture->identifier());

  _loadStatSheet(filename);
}

char* Apsis::Primitives::SpriteSheet::_determineStatSheetFilename(const char* filename) {
  char* stat_sheet = new char[strlen(filename)+3];
  strncpy(stat_sheet, filename, strlen(filename));

  for (unsigned int i = strlen(filename); i > 0; i--) {
    if (stat_sheet[i] == '.') {
      stat_sheet[i] = '\0';
    }
  }

  strcat(stat_sheet, ".json");

  return stat_sheet;
}

void Apsis::Primitives::SpriteSheet::_loadStatSheet(const char* filename) {
  char* stat_sheet = _determineStatSheetFilename(filename);

  Json::Reader reader;
  Json::Value value;

  std::ifstream file(stat_sheet);
  reader.parse(file, value);
  file.close();

  // Sprite List
  // TODO: better handling of invalid values
  for (Json::Value::iterator it = value.begin(); it != value.end(); it++) {
    Sprite* sprite = new Sprite;

    strcpy(sprite->name, (*it)["name"].asCString());
    sprite->x        = (unsigned int)(*it)["x"].asUInt();
    sprite->y        = (unsigned int)(*it)["y"].asUInt();
    sprite->width    = (unsigned int)(*it)["width"].asUInt();
    sprite->height   = (unsigned int)(*it)["height"].asUInt();
    sprite->center_x = (unsigned int)(*it)["center_x"].asUInt();
    sprite->center_y = (unsigned int)(*it)["center_y"].asUInt();

    _sprites.push_back(sprite);
  }

  delete [] stat_sheet;
}

Apsis::Primitives::Texture* Apsis::Primitives::SpriteSheet::texture() {
  return _texture;
}

void Apsis::Primitives::SpriteSheet::textureCoordinates(unsigned int index, float coords[4]) {
  Sprite* sprite = _sprites[index];

  float tu = ((float)sprite->x + 0.1f)  / (float)_width;
  float tv = ((float)sprite->y + 0.1f)  / (float)_height;
  float tu2 = ((float)sprite->x - 0.1f + (float)sprite->width)  / (float)_width;
  float tv2 = ((float)sprite->y - 0.1f + (float)sprite->height)  / (float)_height;

  if (tu < 0.0)  { tu = 0.0; }
  if (tv < 0.0)  { tv = 0.0; }
  if (tu2 < 0.0) { tu2 = 0.0; }
  if (tv2 < 0.0) { tv2 = 0.0; }

  if (tu > 1.0)  { tu = 1.0; }
  if (tv > 1.0)  { tv = 1.0; }
  if (tu2 > 1.0) { tu2 = 1.0; }
  if (tv2 > 1.0) { tv2 = 1.0; }

  coords[0] = (float)tu;
  coords[1] = (float)tv;
  coords[2] = (float)tu2;
  coords[3] = (float)tv2;
}

bool Apsis::Primitives::SpriteSheet::textureCoordinates(const char* name, float coords[4]) {
  for (unsigned int i = 0; i < _sprites.size(); i++) {
    if (strncmp(name, _sprites[i]->name, 64) == 0) {
      textureCoordinates(i, coords);
      return true;
    }
  }
  return false;
}

Apsis::Primitives::Sprite* Apsis::Primitives::SpriteSheet::sprite(unsigned int index) {
  return _sprites[index];
}

Apsis::Primitives::Sprite* Apsis::Primitives::SpriteSheet::sprite(const char* name) {
  for (unsigned int i = 0; i < _sprites.size(); i++) {
    if (strncmp(name, _sprites[i]->name, 64) == 0) {
      return _sprites[i];
    }
  }
  return NULL;
}

int Apsis::Primitives::SpriteSheet::enumerateSprites(const char* wildcard, unsigned int last) {
  int star_pos = -1;

  // Determine where the '*' is in the wildcard
  for (unsigned int i = 0; i < strlen(wildcard); i++) {
    if (wildcard[i] == '*') {
      star_pos = i;
      break;
    }
  }

  if (star_pos == -1) {
    // if we've already found the sprite, don't infinite loop
    if (last > 0) {
      return -1;
    }

    // No star is found... just find by the name
    for (unsigned int i = 0; i < _sprites.size(); i++) {
      if (strncmp(wildcard, _sprites[i]->name, 64) == 0) {
        return i;
      }
    }
    return -1;
  }

  // Look for a wildcard match
  for (unsigned int i = last; i < _sprites.size(); i++) {
    if (strncmp(wildcard, _sprites[i]->name, star_pos) == 0) {
      // Matches in the front
      return i;
    }
  }

  return -1;
}

unsigned int Apsis::Primitives::SpriteSheet::count() {
  return _sprites.size();
}

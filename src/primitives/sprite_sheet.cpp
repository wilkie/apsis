#include "apsis/primitives/sprite_sheet.h"

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include <math.h>
#include <SOIL.h>

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
  char* stat_sheet = new char[strlen(filename)+2];
  strncpy(stat_sheet, filename, strlen(filename));

  for (unsigned int i = strlen(filename); i > 0; i--) {
    if (stat_sheet[i] == '.') {
      stat_sheet[i] = '\0';
    }
  }

  strcat(stat_sheet, ".txt");

  return stat_sheet;
}

void Apsis::Primitives::SpriteSheet::_loadStatSheet(const char* filename) {
  char* stat_sheet = _determineStatSheetFilename(filename);

  FILE* f = fopen(stat_sheet, "rt");

  unsigned int spritesLoaded = 0;
  while(!feof(f)) {
    Sprite* sprite = new Sprite;
    _sprites.push_back(sprite);
    fscanf(f, "%64s %d, %d, %d, %d, %d, %d\n", sprite->name,
                                    &sprite->x,
                                    &sprite->y,
                                    &sprite->width,
                                    &sprite->height,
                                    &sprite->center_x,
                                    &sprite->center_y);
  }

  delete [] stat_sheet;
}

Apsis::Primitives::Texture* Apsis::Primitives::SpriteSheet::texture() {
  return _texture;
}

void Apsis::Primitives::SpriteSheet::textureCoordinates(unsigned int index, float coords[4]) {
  Sprite* sprite = _sprites[index];

  double tu = (double)sprite->x      / (double)_width;
  double tv = (double)sprite->y      / (double)_height;
  double tw = (double)sprite->width  / (double)_width;
  double th = (double)sprite->height / (double)_height;

  coords[0] = (float)tu;
  coords[1] = (float)tv;
  coords[2] = (float)tw;
  coords[3] = (float)th;
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

#include "apsis/primitives/texture.h"

#include "apsis/backend/sdl.h"

#include "SOIL.h"

Apsis::Primitives::Texture::~Texture() {
  if (_counter.isAlone()) {
    glDeleteTextures(1, &_texture);
  }
}

Apsis::Primitives::Texture::Texture(const char* name) {
  _texture = SOIL_load_OGL_texture_with_dimensions(name,
                                                   SOIL_LOAD_RGBA,
                                                   SOIL_CREATE_NEW_ID,
                                                   0,
                                                   &_width,
                                                   &_height);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

Apsis::Primitives::Texture::Texture(unsigned int width,
                                    unsigned int height)
  : _width(width),
    _height(height) {
  glGenTextures(1, &_texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

Apsis::Primitives::Texture::Texture(unsigned int width,
                                    unsigned int height,
                                    void*        data)
  : _width(width),
    _height(height) {
  glGenTextures(1, &_texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

unsigned int Apsis::Primitives::Texture::width() const {
  return _width;
}

unsigned int Apsis::Primitives::Texture::height() const {
  return _height;
}

unsigned int Apsis::Primitives::Texture::identifier() const {
  return this->_texture;
}

void Apsis::Primitives::Texture::blit(unsigned int x,
                                      unsigned int y,
                                      void* data,
                                      unsigned int src_width,
                                      unsigned int src_height) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  x, y, src_width, src_height,
                  GL_RGBA, GL_UNSIGNED_BYTE, data);
}

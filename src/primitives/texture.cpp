#include "apsis/primitives/texture.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include "SOIL.h"

Apsis::Primitives::Texture::Texture(const char* name) {
  glGenTextures(1, &_texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);

  unsigned char* image = SOIL_load_image(name, (int*)&_width, (int*)&_height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

unsigned int Apsis::Primitives::Texture::width() {
  return _width;
}

unsigned int Apsis::Primitives::Texture::height() {
  return _height;
}

Apsis::Primitives::Texture::~Texture() {
  if (_counter.isAlone()) {
    glDeleteTextures(1, &_texture);
  }
}

unsigned int Apsis::Primitives::Texture::identifier() const {
  return this->_texture;
}
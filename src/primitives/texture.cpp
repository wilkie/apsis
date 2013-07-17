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
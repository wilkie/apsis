#include "apsis/primitive/texture.h"

#include "apsis/backend/sdl.h"

#include "apsis/engine/log.h"

#include "SOIL.h"

#define DEBUG_THROW_GL_ERRORS

static void _throwError(const char* function, const char* message) {
  Apsis::Engine::Log::error("Primitive", "Texture", function, message);
}

#ifdef DEBUG_THROW_GL_ERRORS
static void _throwGLError(const char* function) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    const char* errorString;
    switch ( error ) {
      case GL_INVALID_ENUM: errorString = "invalid enumerant"; break;
      case GL_INVALID_VALUE: errorString = "invalid value"; break;
      case GL_INVALID_OPERATION: errorString = "invalid operation"; break;
      case GL_STACK_OVERFLOW: errorString = "stack overflow"; break;
      case GL_STACK_UNDERFLOW: errorString = "stack underflow"; break;
      case GL_OUT_OF_MEMORY: errorString = "out of memory"; break;
      case GL_TABLE_TOO_LARGE: errorString = "table too large"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "invalid framebuffer operation"; break;
//      case GL_TEXTURE_TOO_LARGE: errorString = "texture too large"; break;
      default: errorString = "unknown GL error"; break;
    }
    _throwError(function, errorString);
  }
}
#endif

Apsis::Primitive::Texture::~Texture() {
  if (_counter.isAlone()) {
    glDeleteTextures(1, &_texture);
  }
}

Apsis::Primitive::Texture::Texture(const char* name) {
#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(on stack)");
#endif


  _texture = SOIL_load_OGL_texture_with_dimensions(name,
                                                   SOIL_LOAD_RGBA,
                                                   SOIL_CREATE_NEW_ID,
                                                   0,
                                                   &_width,
                                                   &_height);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(prior thing)");
#endif

  glActiveTexture(GL_TEXTURE0);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glActiveTexture)");
#endif

  glBindTexture(GL_TEXTURE_2D, _texture);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glBindTexture)");
#endif

  glEnable (GL_BLEND);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glEnable)");
#endif

  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glBlendFunc)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glTexParameteri a)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glTexParameteri b)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glTexParameteri c)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glTexParameteri d)");
#endif

}

Apsis::Primitive::Texture::Texture(unsigned int width,
                                    unsigned int height)
  : _width(width),
    _height(height) {
#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(on stack)");
#endif

  glGenTextures(1, &_texture);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glGenTextures)");
#endif

  glActiveTexture(GL_TEXTURE0);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glActiveTexture)");
#endif

  glBindTexture(GL_TEXTURE_2D, _texture);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glBindTexture)");
#endif

  glEnable (GL_BLEND);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glEnable)");
#endif

  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glBlendFunc)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glTexParameteri a)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glTexParameteri b)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glTexParameteri c)");
#endif

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glTexParameteri d)");
#endif

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor_w_h(glTexImage2D)");
#endif
}

Apsis::Primitive::Texture::Texture(unsigned int width,
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

unsigned int Apsis::Primitive::Texture::width() const {
  return _width;
}

unsigned int Apsis::Primitive::Texture::height() const {
  return _height;
}

unsigned int Apsis::Primitive::Texture::identifier() const {
  return this->_texture;
}

void Apsis::Primitive::Texture::blit(unsigned int x,
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

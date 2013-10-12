#include "apsis/primitive/fragment_shader.h"

#include <stdio.h>

#include "apsis/backend/sdl.h"

#include "apsis/engine/log.h"

static void _throwError(const char* function, const char* message) {
  Apsis::Engine::Log::error("Primitive", "FragmentShader", function, message);
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

Apsis::Primitive::FragmentShader::FragmentShader(const char* source) {
#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(on stack)");
#endif

  this->_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glCreateShader)");
#endif

  glShaderSource(this->_fragmentShader, 1, &source, NULL);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glShaderSource)");
#endif

  glCompileShader(this->_fragmentShader);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glCompileShader)");
#endif

  GLint status;
  glGetShaderiv(this->_fragmentShader, GL_COMPILE_STATUS, &status);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("constructor(glGetShaderiv)");
#endif

  if (status != GL_TRUE) {
    // Get errors
    GLchar buffer[2048];
    GLsizei buffer_len;
    glGetShaderInfoLog(this->_fragmentShader, sizeof(buffer), &buffer_len, buffer);
    throw buffer;
  }
}

Apsis::Primitive::FragmentShader::~FragmentShader() {
  if (_counter.isAlone()) {
    //glDeleteShader(this->_fragmentShader);

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("destructor(glDeleteShader)");
#endif

  }
}

Apsis::Primitive::FragmentShader Apsis::Primitive::FragmentShader::fromFile(const char* path) {
  Engine::Log::printf("Loading Fragment Shader %s\n", path);
  FILE* f = fopen(path, "rb");
  if (f == NULL) {
    throw "Shader not found.";
  }

  fseek(f, 0, SEEK_END);
  unsigned long file_length = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (file_length == 0) {
    return FragmentShader("");
  }

  char* source = new char[file_length + 1];
  if (source == NULL) {
    throw "Memory depleted";
  }

  fread(source, 1, file_length, f);

  source[file_length] = 0;

  fclose(f);

  FragmentShader ret = FragmentShader(source);
  delete [] source;
  return ret;
}

unsigned int Apsis::Primitive::FragmentShader::identifier() const {
  return this->_fragmentShader;
}

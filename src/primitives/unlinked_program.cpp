#include "apsis/primitives/unlinked_program.h"

#include "apsis/backend/sdl.h"

#include "apsis/engine/log.h"

#define DEBUG_THROW_GL_ERRORS

static void _throwError(const char* function, const char* message) {
  Apsis::Engine::Log::error("Primitives", "UnlinkedProgram", function, message);
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

Apsis::Primitives::UnlinkedProgram::UnlinkedProgram() :
  _linked(false) {
  this->_program = glCreateProgram();
}

Apsis::Primitives::UnlinkedProgram::~UnlinkedProgram() {
  if (_counter.isAlone()) {
    if (!(this->_linked)) {
      glDeleteProgram(this->_program);

#ifdef DEBUG_THROW_GL_ERRORS
      _throwGLError("destructor(glDeleteProgram)");
#endif
    }
  }
}

void Apsis::Primitives::UnlinkedProgram::attach(VertexShader& vertexShader) {
#ifdef DEBUG_THROW_GL_ERRORS
      _throwGLError("attach_vs(on stack)");
#endif

  if (_linked) throw "Program already linked. Cannot attach vertex shader.";
  _vertexShaders.push_back(vertexShader);
  glAttachShader(this->_program, vertexShader.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("attach_vs(glAttachShader)");
#endif
}

void Apsis::Primitives::UnlinkedProgram::attach(FragmentShader& fragmentShader) {
  if (_linked) throw "Program already linked. Cannot attach fragment shader.";
  _fragmentShaders.push_back(fragmentShader);
  glAttachShader(this->_program, fragmentShader.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("attach_fs(glAttachShader)");
#endif
}

void Apsis::Primitives::UnlinkedProgram::defineFragmentOutput(const char* name) {
  //glBindFragDataLocation(this->_program, 0, name);
}

Apsis::Primitives::Program Apsis::Primitives::UnlinkedProgram::link() {
  if (_linked) throw "Program already linked. Cannot link again.";
  glLinkProgram(_program);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("link(glLinkProgram)");
#endif

  GLint status;
  glGetProgramiv(_program, GL_LINK_STATUS, &status);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("link(glGetProgramiv)");
#endif

  if (status != GL_TRUE) {
    throw "Cannot link program";
  }
  _linked = true;
  return Program(_program, _vertexShaders, _fragmentShaders);
}

#include "apsis/primitives/program.h"

#include "apsis/backend/sdl.h"

#include "apsis/engine/log.h"

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#define DEBUG_THROW_GL_ERRORS

static void _throwError(const char* function, const char* message) {
  Apsis::Engine::Log::error("Primitives", "Program", function, message);
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

Apsis::Primitives::Program::Program(unsigned int program,
                                    std::vector<const VertexShader*> vertexShaders,
                                    std::vector<const FragmentShader*> fragmentShaders) :
  _program(program),
  _vertexShaders(vertexShaders),
  _fragmentShaders(fragmentShaders) {
}

Apsis::Primitives::Program::~Program() {
  if (_counter.isAlone() && this->_program != 0) {
    glDeleteProgram(this->_program);
  }

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("destructor(glDeleteProgram)");
#endif
}

unsigned int Apsis::Primitives::Program::identifier() const {
  return this->_program;
}
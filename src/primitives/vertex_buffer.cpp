#include "apsis/primitives/vertex_buffer.h"

#include "apsis/backend/sdl.h"

#include "apsis/engine/log.h"

static void _throwError(const char* function, const char* message) {
  Apsis::Engine::Log::error("Primitives", "VertexBuffer", function, message);
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

Apsis::Primitives::VertexBuffer::Target::Targets
Apsis::Primitives::VertexBuffer::target() const {
  return _target;
}

Apsis::Primitives::VertexBuffer::VertexBuffer(Target::Targets target)
  : _target(target) {
  glGenBuffers(1, &this->_vbo);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("~constructor");
#endif
}

Apsis::Primitives::VertexBuffer::~VertexBuffer() {
  if (_counter.isAlone()) {
    //glDeleteBuffers(1, &this->_vbo);

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("~destructor");
#endif
  }
}

void Apsis::Primitives::VertexBuffer::defineInput(const char*    name,
                                                  const Program& program,
                                                  unsigned int   numberOfComponents,
                                                  Type::Types    type,
                                                  bool           normalized,
                                                  unsigned int   stride,
                                                  unsigned int   offset) {
  glUseProgram(program.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("defineInput(glUseProgram)");
#endif

  GLint posAttrib = glGetAttribLocation(program.identifier(), name);
  if (posAttrib < 0) {
    char error[1024];
    sprintf(error, "Program.defineInput: Error: Cannot define input %.250s. Input not found.", name);
    throw error;
  }

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("defineInput(glGetAttribLocation)");
#endif

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("defineInput(glBindBuffer)");
#endif

  glEnableVertexAttribArray(posAttrib);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("defineInput(glEnableVertexAttribArray)");
#endif

  GLenum gltype;
  GLuint glsize;

  if (type == Type::Float) {
    gltype = (GLenum)GL_FLOAT;
    glsize = sizeof(float);
  }

  glVertexAttribPointer(posAttrib,
                        numberOfComponents,
                        gltype,
                        normalized,
                        (GLsizei)(stride * glsize),
                        (const GLvoid*)(size_t)(offset * glsize));

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("defineInput(glVertexAttribPointer)");
#endif

#ifdef JS_MODE
  AttributeInfo attr;

  attr.attribute = posAttrib;
  attr.normalized = normalized;
  attr.numberOfComponents = numberOfComponents;
  attr.type = type;
  attr.offset = offset;
  attr.stride = stride;

  _attributes.push_back(attr);
#endif
}

void Apsis::Primitives::VertexBuffer::transfer(const float* elements, unsigned int count) {
  GLenum target = GL_ARRAY_BUFFER;
  if (_target == Target::Data) {
    target = GL_ARRAY_BUFFER;
  }
  else if (_target == Target::Elements) {
    target = GL_ELEMENT_ARRAY_BUFFER;
  }
  glBindBuffer(target, this->_vbo);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("transferf(glBindBuffer)!");
#endif

  glBufferData(target, count * sizeof(GLfloat), elements, GL_STATIC_DRAW);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("transferf(glBufferData)");
#endif

  _count = count;
}

void Apsis::Primitives::VertexBuffer::transfer(const unsigned short* elements, unsigned int count) {
  GLenum target = GL_ARRAY_BUFFER;
  if (_target == Target::Data) {
    target = GL_ARRAY_BUFFER;
  }
  else if (_target == Target::Elements) {
    target = GL_ELEMENT_ARRAY_BUFFER;
  }
  glBindBuffer(target, this->_vbo);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("transferi(glBindBuffer)");
#endif

  glBufferData(target, count * sizeof(GLushort), elements, GL_STATIC_DRAW);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("transferi(glBufferData)");
#endif

  _count = count;
}

void Apsis::Primitives::VertexBuffer::transfer(const unsigned short* elements, unsigned int count, unsigned int at) {
  GLenum target = GL_ARRAY_BUFFER;
  if (_target == Target::Data) {
    target = GL_ARRAY_BUFFER;
  }
  else if (_target == Target::Elements) {
    target = GL_ELEMENT_ARRAY_BUFFER;
  }
  glBindBuffer(target, this->_vbo);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("transferisub(glBindBuffer)");
#endif

  glBufferSubData(target, (GLintptr)(at * sizeof(GLushort)), count, elements);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("transferisub(glBufferData)");
#endif
}

unsigned int Apsis::Primitives::VertexBuffer::count() const {
  return _count;
}

unsigned int Apsis::Primitives::VertexBuffer::identifier() const {
  return this->_vbo;
}

void Apsis::Primitives::VertexBuffer::bind() const {
  if (_target == Target::Data) {
    glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
  }
  else if (_target == Target::Elements) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo);
  }

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bind(glBindBuffer)");
#endif

#ifdef JS_MODE
  for (unsigned int i = 0; i < _attributes.size(); i++) {
    const AttributeInfo& attr = _attributes[i];

    glEnableVertexAttribArray(attr.attribute);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bind(glEnableVertexAttribArray)");
#endif

    GLenum gltype;
    GLuint glsize;

    if (attr.type == Type::Float) {
      gltype = (GLenum)GL_FLOAT;
      glsize = sizeof(float);
    }

    glVertexAttribPointer(attr.attribute,
                          attr.numberOfComponents,
                          gltype,
                          attr.normalized,
                          (GLsizei)(attr.stride * glsize),
                          (const GLvoid*)(size_t)(attr.offset * glsize));

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("bind(glVertexAttribPointer)");
#endif
  }
#endif
}

#include "apsis/primitives/vertex_array.h"

#include "apsis/backend/sdl.h"

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "apsis/engine/log.h"

unsigned int Apsis::Primitives::VertexArray::_current_vao = 0xffffffff;

static void _throwError(const char* function, const char* message) {
  Apsis::Engine::Log::error("Primitives", "VertexArray", function, message);
}

#ifdef DEBUG_THROW_GL_ERRORS
static void _throwGLError(const char* function) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    _throwError(function, (char*)gluErrorString(error));
  }
}
#endif

Apsis::Primitives::VertexArray::VertexArray() {
#ifdef JS_MODE // Older GL / GLES2
  glGenBuffers(1, &this->_vao);
#else
  glGenVertexArrays(1, &this->_vao);
#endif

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("~constructor");
#endif
}

Apsis::Primitives::VertexArray::~VertexArray() {
  if (_counter.isAlone()) {
#ifdef JS_MODE // Older GL / GLES2
  glDeleteBuffers(1, &this->_vao);
#else
  glDeleteVertexArrays(1, &this->_vao);
#endif

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("~destructor");
#endif
  }
}

void Apsis::Primitives::VertexArray::_bind() const {
#ifdef JS_MODE
  if (_current_vao != this->_vao) {
    glBindBuffer(GL_ARRAY_BUFFER, this->_vao);
    _current_vao = this->_vao;
  }
#else
  glBindVertexArray(this->_vao);
#endif

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("use");
#endif
}

void Apsis::Primitives::VertexArray::use() {
  _bind();
}

void Apsis::Primitives::VertexArray::bindElements(VertexBuffer& buffer) {
  _bind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bindElements");
#endif

  if (_elementBuffer.size() > 0) {
    _elementBuffer.clear();
  }
  _elementBuffer.push_back(buffer);
}

void Apsis::Primitives::VertexArray::useProgram(Program& program) {
  _bind();
  glUseProgram(program.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("useProgram");
#endif

  // Only add if not already there
  for(unsigned int i = 0; i < _programs.size(); i++) {
    if (_programs[i].identifier() == program.identifier()) {
      return;
    }
  }
  _programs.push_back(program);
}

void Apsis::Primitives::VertexArray::draw() const {
  _bind();

  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("draw");
#endif
}

void Apsis::Primitives::VertexArray::drawRange(unsigned int start, unsigned int count) const {
  _bind();

  if (_elementBuffer.size() > 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(start * sizeof(GLuint)));

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("drawRange");
#endif
}

void Apsis::Primitives::VertexArray::drawQuads() const {
  _bind();

  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_QUADS, count, GL_UNSIGNED_INT, 0);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("drawQuads");
#endif
}

void Apsis::Primitives::VertexArray::drawQuadsRange(unsigned int start, unsigned int count) const {
  _bind();

  if (_elementBuffer.size() > 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_QUADS, count, GL_UNSIGNED_INT, (void*)(start * sizeof(GLuint)));
}

int Apsis::Primitives::VertexArray::defineUniform(const char* name, Program& program) {
  useProgram(program);
  GLint uniform = glGetUniformLocation(program.identifier(), name);
  if (uniform < 0) {
    char error[1024];
    sprintf(error, "Cannot upload uniform %.250s. Uniform not found.", name);
    _throwError("defineUniform", error);
  }
  std::string key = name;
  _uniforms[name] = uniform;
  return uniform;
}

void Apsis::Primitives::VertexArray::uploadUniform(const char* name,
                                                   const Matrix& mat) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, mat);
}

void Apsis::Primitives::VertexArray::uploadUniform(int identifier,
                                                   const Matrix& mat) const {
  glUniformMatrix4fv(identifier, 1, GL_FALSE, (float*)&mat.value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (Matrix)");
#endif
}

void Apsis::Primitives::VertexArray::uploadUniform(const char* name,
                                                   int         value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Apsis::Primitives::VertexArray::uploadUniform(int identifier,
                                                   int value) const {
  glUniform1i(identifier, value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (int)");
#endif
}

void Apsis::Primitives::VertexArray::uploadUniform(const char* name,
                                                   float       value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Apsis::Primitives::VertexArray::uploadUniform(int   identifier,
                                                   float value) const {
  glUniform1f(identifier, value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (float)");
#endif
}

void Apsis::Primitives::VertexArray::uploadUniform(const char* name,
                                                   const Vector3& value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Apsis::Primitives::VertexArray::uploadUniform(int         identifier,
                                                   const Vector3& value) const {
  glUniform3fv(identifier, 1, (float*)&value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (Vector3)");
#endif
}

void Apsis::Primitives::VertexArray::uploadUniform(const char* name,
                                                   const Vector4& value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Apsis::Primitives::VertexArray::uploadUniform(int         identifier,
                                                   const Vector4& value) const {
  glUniform4fv(identifier, 1, (float*)&value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (Vector4)");
#endif
}

void Apsis::Primitives::VertexArray::bindTexture(unsigned int slot,
                                                 const Texture& texture) {
  std::map<unsigned int, Texture>::iterator it = _textures.find(slot);
  if (it != _textures.end()) {
    _textures.erase(it);
  }

  _textures.insert(_textures.begin(),
                   std::pair<unsigned int, Texture>(slot, texture));

  if (slot > 31) {
    _throwError("bindTexture", "Cannot bind texture. Slot too high.");
  }

  glActiveTexture(GL_TEXTURE0  + slot);
  glBindTexture(GL_TEXTURE_2D, texture.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bindTexture");
#endif
}

unsigned int Apsis::Primitives::VertexArray::identifier() const {
  return this->_vao;
}

void Apsis::Primitives::VertexArray::_bindTextures() const {
  std::map<unsigned int, Texture>::const_iterator it;

  for (it = _textures.begin(); it != _textures.end(); ++it) {
    unsigned int slot = it->first;
    const Texture& texture = it->second;

    glActiveTexture(GL_TEXTURE0  + slot);
    glBindTexture(GL_TEXTURE_2D, texture.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("_bindTextures");
#endif
  }
}

void Apsis::Primitives::VertexArray::bindProgram() const {
  if (_programs.size() > 0) {
    glUseProgram(_programs[0].identifier());
  }
  else {
    glUseProgram(0);
  }

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bindProgram");
#endif
}

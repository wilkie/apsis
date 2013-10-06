#include "apsis/primitives/vertex_array.h"

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

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "apsis/engine/log.h"

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
  glGenVertexArrays(1, &this->_vao);
}

Apsis::Primitives::VertexArray::~VertexArray() {
  if (_counter.isAlone()) {
    glDeleteVertexArrays(1, &this->_vao);
  }
}

void Apsis::Primitives::VertexArray::use() {
  glBindVertexArray(this->_vao);
}

void Apsis::Primitives::VertexArray::bindElements(VertexBuffer& buffer) {
  glBindVertexArray(this->_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.identifier());

  if (_elementBuffer.size() > 0) {
    _elementBuffer.clear();
  }
  _elementBuffer.push_back(buffer);
}

void Apsis::Primitives::VertexArray::useProgram(Program& program) {
  glBindVertexArray(this->_vao);
  glUseProgram(program.identifier());

  // Only add if not already there
  for(unsigned int i = 0; i < _programs.size(); i++) {
    if (_programs[i].identifier() == program.identifier()) {
      return;
    }
  }
  _programs.push_back(program);
}

void Apsis::Primitives::VertexArray::draw() const {
  glBindVertexArray(this->_vao);

  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void Apsis::Primitives::VertexArray::drawRange(unsigned int start, unsigned int count) const {
  glBindVertexArray(this->_vao);

  if (_elementBuffer.size() > 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(start * sizeof(GLuint)));
}

void Apsis::Primitives::VertexArray::drawQuads() const {
  glBindVertexArray(this->_vao);

  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer[0].identifier());
  }

  _bindTextures();

  glDrawElements(GL_QUADS, count, GL_UNSIGNED_INT, 0);
}

void Apsis::Primitives::VertexArray::drawQuadsRange(unsigned int start, unsigned int count) const {
  glBindVertexArray(this->_vao);

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
  printf("%s\n", name);
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

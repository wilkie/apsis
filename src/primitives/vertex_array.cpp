#include "iso-tasty/primitives/vertex_array.h"

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

IsoTasty::Primitives::VertexArray::VertexArray() {
  glGenVertexArrays(1, &this->_vao);
}

IsoTasty::Primitives::VertexArray::~VertexArray() {
  if (_counter.isAlone()) {
    glDeleteVertexArrays(1, &this->_vao);
  }
}

void IsoTasty::Primitives::VertexArray::use() {
  glBindVertexArray(this->_vao);
}

void IsoTasty::Primitives::VertexArray::bindElements(VertexBuffer& buffer) {
  glBindVertexArray(this->_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.identifier());

  if (_elementBuffer.size() > 0) {
    _elementBuffer.clear();
  }
  _elementBuffer.push_back(buffer);
}

void IsoTasty::Primitives::VertexArray::useProgram(Program& program) {
  glBindVertexArray(this->_vao);
  glUseProgram(program.identifier());

  // Only add if not already there
  for(unsigned int i = 0; i < _programs.size(); i++){
    if (_programs[i].identifier() == program.identifier()) {
      return;
    }
  }
  _programs.push_back(program);
}

void IsoTasty::Primitives::VertexArray::draw() {
  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
  }

  glBindVertexArray(this->_vao);
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

int IsoTasty::Primitives::VertexArray::defineUniform(const char* name, Program& program) {
  useProgram(program);
  GLint uniform = glGetUniformLocation(program.identifier(), name);
  if (uniform < 0) {
    throw "Cannot upload uniform. Uniform not found.";
  }
  std::string key = name;
  _uniforms[name] = uniform;
  return uniform;
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name, glm::mat4& mat) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, mat);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int identifier, glm::mat4& mat) const {
  glUniformMatrix4fv(identifier, 1, GL_FALSE, glm::value_ptr(mat));
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name,
                                                      int         value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int identifier,
                                                      int value) const {
  glUniform1i(identifier, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name,
                                                      float       value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int   identifier,
                                                      float value) const {
  glUniform1f(identifier, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name,
                                                      glm::vec3&  value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int        identifier,
                                                      glm::vec3& value) const {
  glUniform3fv(identifier, 1, glm::value_ptr(value));
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name,
                                                      glm::vec4&  value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int        identifier,
                                                      glm::vec4& value) const {
  glUniform4fv(identifier, 1, glm::value_ptr(value));
}

void IsoTasty::Primitives::VertexArray::bindTexture(unsigned int slot, Texture& texture) {
  std::map<unsigned int, Texture>::iterator it = _textures.find(slot);
  if (it != _textures.end()) {
    _textures.erase(it);
  }

  _textures.insert(_textures.begin(), std::pair<unsigned int, Texture>(slot, texture));

  if (slot > 31) {
    throw "Cannot bind texture. Slot too high.";
  }

  glActiveTexture(GL_TEXTURE0  + slot);
  glBindTexture(GL_TEXTURE_2D, texture.identifier());
}

unsigned int IsoTasty::Primitives::VertexArray::identifier() const {
  return this->_vao;
}
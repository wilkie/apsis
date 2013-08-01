#include "apsis/primitives/vertex_shader.h"

#include <stdio.h>

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

Apsis::Primitives::VertexShader::VertexShader(const char* source) {
  this->_vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(this->_vertexShader, 1, &source, NULL);
  glCompileShader(this->_vertexShader);
  GLint status;
  glGetShaderiv(this->_vertexShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    // Get errors
    GLchar buffer[2048];
    GLsizei buffer_len;
    glGetShaderInfoLog(this->_vertexShader, sizeof(buffer), &buffer_len, buffer);
    throw buffer;
  }
}

Apsis::Primitives::VertexShader::~VertexShader() {
  if (_counter.isAlone()) {
    glDeleteShader(this->_vertexShader);
  }
}

Apsis::Primitives::VertexShader Apsis::Primitives::VertexShader::fromFile(const char* path) {
  FILE* f = fopen(path, "rb");
  if (f == NULL) {
    throw "Shader not found.";
  }

  fseek(f, 0, SEEK_END);
  unsigned long file_length = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (file_length == 0) {
    return VertexShader("");
  }

  char* source = new char[file_length + 1];
  if (source == NULL) {
    throw "Memory depleted";
  }

  fread(source, 1, file_length, f);

  source[file_length] = 0;

  fclose(f);

  VertexShader ret = VertexShader(source);
  delete [] source;
  return ret;
}

unsigned int Apsis::Primitives::VertexShader::identifier() const {
  return this->_vertexShader;
}

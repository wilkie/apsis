#include "apsis/primitives/fragment_shader.h"

#include <stdio.h>

#include "apsis/backend/sdl.h"

Apsis::Primitives::FragmentShader::FragmentShader(const char* source) {
  this->_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(this->_fragmentShader, 1, &source, NULL);
  glCompileShader(this->_fragmentShader);
  GLint status;
  glGetShaderiv(this->_fragmentShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    // Get errors
    GLchar buffer[2048];
    GLsizei buffer_len;
    glGetShaderInfoLog(this->_fragmentShader, sizeof(buffer), &buffer_len, buffer);
    throw buffer;
  }
}

Apsis::Primitives::FragmentShader::~FragmentShader() {
  if (_counter.isAlone()) {
    //glDeleteShader(this->_fragmentShader);
  }
}

Apsis::Primitives::FragmentShader Apsis::Primitives::FragmentShader::fromFile(const char* path) {
  printf("Loading Fragment Shader %s\n", path);
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

unsigned int Apsis::Primitives::FragmentShader::identifier() const {
  return this->_fragmentShader;
}

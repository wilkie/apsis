#include "iso-tasty/primitives/fragment_shader.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

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

IsoTasty::Primitives::FragmentShader::FragmentShader(const char* source) {
  this->_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(this->_fragmentShader, 1, &source, NULL);
  glCompileShader(this->_fragmentShader);
}

IsoTasty::Primitives::FragmentShader::~FragmentShader() {
  if (_counter.isAlone()) {
    //glDeleteShader(this->_fragmentShader);
  }
}

IsoTasty::Primitives::FragmentShader IsoTasty::Primitives::FragmentShader::fromFile(const char* path) {
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

  FragmentShader& ret = FragmentShader(source);
  delete [] source;
  return ret;
}

unsigned int IsoTasty::Primitives::FragmentShader::identifier() const {
  return this->_fragmentShader;
}
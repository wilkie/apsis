#include "apsis/engine/graphics.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

std::vector<Apsis::Engine::Graphics*> Apsis::Engine::Graphics::_graphics_engines;

Apsis::Engine::Graphics::Graphics(const Apsis::Settings::Video& settings)
  : _videoSettings(settings),
    _font(NULL),
    _camera(*(Primitives::Vector2*)glm::value_ptr(glm::vec2(0,0)), 0.0f),
    _default(NULL) {
}

Apsis::Engine::Graphics& Apsis::Engine::Graphics::basic(const Apsis::Settings::Video& settings) {
  Apsis::Engine::Graphics* ge = new Apsis::Engine::Graphics(settings);
  _graphics_engines.push_back(ge);
  return *ge;
}

const Apsis::Settings::Video& Apsis::Engine::Graphics::settings() const {
  return _videoSettings;
}

void Apsis::Engine::Graphics::font(unsigned int id) {
  _font = &Apsis::Sprite::Font::get(id);
}

const Apsis::Sprite::Font& Apsis::Engine::Graphics::font() const {
  if (_font == NULL) {
    if (_default == NULL) {
      // Create default font!
      const Apsis::Sprite::Font& font = Apsis::Sprite::Font::load("Arial");
      _default = &font;
    }

    return *_default;
  }

  return *_font;
}

void Apsis::Engine::Graphics::sheet(unsigned int id) {
  _sheet = &Apsis::Sprite::Sheet::loaded(id);
}

/*
  *  Returns the current sprite sheet.
  */
// TODO: Sprite sheets can be used simultaneously.
const Apsis::Sprite::Sheet& Apsis::Engine::Graphics::sheet() const {
  return *_sheet;
}

void Apsis::Engine::Graphics::draw(float x, float y, const char* string) const {
  const Apsis::Sprite::Font& font = this->font();

  Primitives::Vector4 color = {0.58f, 0.58f, 0.89f, 1.0f};
  //font.draw(matrix, camera, color, 100.0f, 100.0f, "Hello World How Are You?");
  font.draw(_projection, _camera, color, x, y, string);
  //font.draw(x, y, string);
}

void Apsis::Engine::Graphics::draw(float x,
                                   float y,
                                   unsigned int sheet_id,
                                   unsigned int sprite_id) const {
  const Apsis::Sprite::Sheet& sheet = this->sheet();

  glm::mat4 model = glm::translate(glm::mat4(1.0),
    glm::vec3(x, 0.0, y));

  const Primitives::Matrix& model_matrix
    = *(const Primitives::Matrix*)glm::value_ptr(model);

  sheet.draw(sprite_id, _projection, _camera, model_matrix);
}

void Apsis::Engine::Graphics::clear() const {
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Apsis::Engine::Graphics::perspective() {
  float width  = (float)_videoSettings.resolutionX;
  float height = (float)_videoSettings.resolutionY;

  float aspect = width / height;

  float nearf = 1;
  float farf = 20.0;

  float fov = 45.0f;

  float top = tanf(fov * 0.5f) * nearf;
  float bottom = -top;

  float left = aspect * bottom;
  float right = aspect * top;

  glm::mat4 projection;
  projection = glm::perspective(fov, aspect, nearf, farf);

  _projection = *(const Primitives::Matrix*)glm::value_ptr(projection);
}

void Apsis::Engine::Graphics::orthographic() {
  float width  = (float)_videoSettings.resolutionX;
  float height = (float)_videoSettings.resolutionY;

  float half_height = height / 2.0f;
  float half_width  = width  / 2.0f;

  glm::mat4 projection;
  projection = glm::ortho(-half_width, half_width, -half_height, half_height);

  _projection = *(const Primitives::Matrix*)glm::value_ptr(projection);
}

const Apsis::Primitives::Matrix& Apsis::Engine::Graphics::projection() const {
  return _projection;
}

const Apsis::Primitives::Matrix& Apsis::Engine::Graphics::view() const {
  return _camera.view();
}

const Apsis::Primitives::Camera& Apsis::Engine::Graphics::camera() const {
  return _camera;
}

void Apsis::Engine::Graphics::camera(Primitives::Camera& camera) {
  _camera = camera;
}
#include "apsis/engine/graphics.h"

std::vector<Apsis::Engine::Graphics*> Apsis::Engine::Graphics::_graphics_engines;

Apsis::Engine::Graphics::Graphics()
  : _font(NULL),
    _default(NULL) {
}

Apsis::Engine::Graphics& Apsis::Engine::Graphics::basic() {
  Apsis::Engine::Graphics* ge = new Apsis::Engine::Graphics();
  _graphics_engines.push_back(ge);
  return *ge;
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

void Apsis::Engine::Graphics::draw(float x, float y, const char* string) const {
  const Apsis::Sprite::Font& font = this->font();

  // font.draw(x, y, string);
}
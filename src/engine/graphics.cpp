#include "apsis/engine/graphics.h"

std::vector<Apsis::Engine::Graphics*> Apsis::Engine::Graphics::_graphics_engines;

Apsis::Engine::Graphics& Apsis::Engine::Graphics::basic() {
  Apsis::Engine::Graphics* ge = new Apsis::Engine::Graphics();
  _graphics_engines.push_back(ge);
  return *ge;
}
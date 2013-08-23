#include "apsis/engine/system.h"

Apsis::Engine::System::System() 
  : _input(*(new Apsis::Engine::Input())) {
}

Apsis::Engine::System::~System() {
  delete &_input;
}

const Apsis::Engine::Input& Apsis::Engine::System::inputEngine() const {
  return _input;
}

Apsis::Engine::Input& Apsis::Engine::System::inputEngine() {
  return _input;
}
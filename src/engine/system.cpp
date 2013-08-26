#include "apsis/engine/system.h"

Apsis::Engine::System::System() 
  : _input(*(new Apsis::Engine::Input())),
    _objects(*(new Apsis::Engine::Object())) {
}

Apsis::Engine::System::~System() {
  delete &_input;
  delete &_objects;
}

const Apsis::Engine::Input& Apsis::Engine::System::inputEngine() const {
  return _input;
}

Apsis::Engine::Input& Apsis::Engine::System::inputEngine() {
  return _input;
}

const Apsis::Engine::Object& Apsis::Engine::System::objectEngine() const {
  return _objects;
}

Apsis::Engine::Object& Apsis::Engine::System::objectEngine() {
  return _objects;
}
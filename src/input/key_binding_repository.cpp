#include "iso-tasty/input/key_binding_repository.h"

Apsis::KeyBindingRepository::KeyBindingRepository() {
}

Apsis::KeyBindingRepository::~KeyBindingRepository() {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    delete _bindings[i];
  }
}

void Apsis::KeyBindingRepository::registerEvent(const char* name,
                                                   int event,
                                                   Apsis::KeyBinding* primary,
                                                   Apsis::KeyBinding* secondary) {
  if (event < 0) {
    throw "The event identifier cannot be less than or equal to zero";
  }

  Binding* binding = new Binding();

  binding->name  = name;
  binding->value = event;

  if (primary == NULL) {
    binding->primary.key = Apsis::Key::NONE;
  }
  else {
    binding->primary = *primary;
  }

  if (secondary == NULL) {
    binding->secondary.key = Apsis::Key::NONE;
  }
  else {
    binding->secondary = *secondary;
  }

  _bindings.push_back(binding);
}

void Apsis::KeyBindingRepository::rebindPrimary(const char* name,
                                                   KeyBinding* primary) {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    if (strncmp(_bindings[i]->name, name, 128) == 0) {
      if (primary == NULL) {
        _bindings[i]->primary.key = Apsis::Key::NONE;
      }
      else {
        _bindings[i]->primary = *primary;
      }
      return;
    }
  }
}

void Apsis::KeyBindingRepository::rebindSecondary(const char* name,
                                                     KeyBinding* secondary) {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    if (strncmp(_bindings[i]->name, name, 128) == 0) {
      if (secondary == NULL) {
        _bindings[i]->secondary.key = Apsis::Key::NONE;
      }
      else {
        _bindings[i]->secondary = *secondary;
      }
      return;
    }
  }
}

int Apsis::KeyBindingRepository::yieldEvent(KeyBinding* binding) {
  int event = 0;

  for (unsigned int i = 0; i < _bindings.size(); i++) {
    if (_bindings[i]->primary.key     != Apsis::Key::NONE &&
        _bindings[i]->primary.key     == binding->key        &&
        _bindings[i]->primary.shift   == binding->shift      &&
        _bindings[i]->primary.control == binding->control    &&
        _bindings[i]->primary.alt     == binding->alt) {
      event = _bindings[i]->value;
      break;
    }

    if (_bindings[i]->secondary.key     != Apsis::Key::NONE &&
        _bindings[i]->secondary.key     == binding->key        &&
        _bindings[i]->secondary.shift   == binding->shift      &&
        _bindings[i]->secondary.control == binding->control    &&
        _bindings[i]->secondary.alt     == binding->alt) {
      event = _bindings[i]->value;
      break;
    }
  }

  if (event == 0) {
    return 0;
  }

  return event;
}

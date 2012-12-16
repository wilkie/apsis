#include "iso-tasty/key_binding_repository.h"

IsoTasty::KeyBindingRepository::KeyBindingRepository() {
}

IsoTasty::KeyBindingRepository::~KeyBindingRepository() {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    delete _bindings[i];
  }
}

void IsoTasty::KeyBindingRepository::registerEvent(const char* name,
                                                   int event,
                                                   IsoTasty::KeyBinding* primary,
                                                   IsoTasty::KeyBinding* secondary) {
  if (event < 0) {
    throw "The event identifier cannot be less than or equal to zero";
  }

  Binding* binding = new Binding();

  binding->name  = name;
  binding->value = event;

  if (primary == NULL) {
    binding->primary.key = IsoTasty::Key::NONE;
  }
  else {
    binding->primary = *primary;
  }

  if (secondary == NULL) {
    binding->secondary.key = IsoTasty::Key::NONE;
  }
  else {
    binding->secondary = *secondary;
  }

  _bindings.push_back(binding);
}

void IsoTasty::KeyBindingRepository::rebindPrimary(const char* name,
                                                   KeyBinding* primary) {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    if (strncmp(_bindings[i]->name, name, 128) == 0) {
      if (primary == NULL) {
        _bindings[i]->primary.key = IsoTasty::Key::NONE;
      }
      else {
        _bindings[i]->primary = *primary;
      }
      return;
    }
  }
}

void IsoTasty::KeyBindingRepository::rebindSecondary(const char* name,
                                                     KeyBinding* secondary) {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    if (strncmp(_bindings[i]->name, name, 128) == 0) {
      if (secondary == NULL) {
        _bindings[i]->secondary.key = IsoTasty::Key::NONE;
      }
      else {
        _bindings[i]->secondary = *secondary;
      }
      return;
    }
  }
}

int IsoTasty::KeyBindingRepository::yieldEvent(KeyBinding* binding) {
  int event = 0;

  for (unsigned int i = 0; i < _bindings.size(); i++) {
    if (_bindings[i]->primary.key     == binding->key     &&
        _bindings[i]->primary.shift   == binding->shift   &&
        _bindings[i]->primary.control == binding->control &&
        _bindings[i]->primary.alt     == binding->alt) {
      event = _bindings[i]->value;
      break;
    }

    if (_bindings[i]->secondary.key     == binding->key     &&
        _bindings[i]->secondary.shift   == binding->shift   &&
        _bindings[i]->secondary.control == binding->control &&
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

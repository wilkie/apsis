#include "iso-tasty/input_handler.h"

IsoTasty::InputHandler::InputHandler() {
}

IsoTasty::InputHandler::~InputHandler() {
  for (unsigned int i = 0; i < _bindings.size(); i++) {
    delete _bindings[i];
  }
}

void IsoTasty::InputHandler::registerEvent(const char* name,
                                           int value,
                                           IsoTasty::KeyBinding* primary,
                                           IsoTasty::KeyBinding* secondary) {
  Binding* binding = new Binding();

  binding->name      = name;
  binding->value     = value;

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

void IsoTasty::InputHandler::rebindPrimary(const char* name,
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

void IsoTasty::InputHandler::rebindSecondary(const char* name,
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

int IsoTasty::InputHandler::yieldEvent(bool pressed, KeyBinding* binding) {
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

  if (pressed) {
    _held.push_back(event);
  }
  else {
    for (unsigned int i = 0; i < _held.size(); i++) {
      if (_held[i] == event) {
        _held.erase(_held.begin() + i);
        break;
      }
    }
  }

  return event;
}

bool IsoTasty::InputHandler::isEventHeld(int event) {
  for (unsigned int i = 0; i < _held.size(); i++) {
    if (_held[i] == event) {
      return true;
    }
  }
  return false;
}

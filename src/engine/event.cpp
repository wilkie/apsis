#include "apsis/engine/event.h"

Apsis::Engine::Event::Type::Types Apsis::Engine::Event::type() const {
  return _type;
}

Apsis::Engine::Event::SystemEvent::Types Apsis::Engine::Event::systemEvent() const {
  if (_type == Apsis::Engine::Event::Type::SystemEvent) {
    return _payload.systemEvent;
  }

  // TODO: i18n?
  throw "Event is not a system event.";
}

void Apsis::Engine::Event::systemEvent(Apsis::Engine::Event::SystemEvent::Types value) {
  _payload.systemEvent = value;
  _type = Apsis::Engine::Event::Type::SystemEvent;
}

const Apsis::Input::Binding Apsis::Engine::Event::binding() const {
  if (_type == Apsis::Engine::Event::Type::Press ||
      _type == Apsis::Engine::Event::Type::Release) {
    return Apsis::Input::Binding(_payload.binding.code,
                                 _payload.binding.shift,
                                 _payload.binding.control,
                                 _payload.binding.alt);
  }

  // TODO: i18n?
  throw "Event is not an input event.";
}

void Apsis::Engine::Event::binding(const Apsis::Input::Binding& binding,
                                   bool pressed) {
  _payload.binding.code    = binding.key();
  _payload.binding.alt     = binding.alt();
  _payload.binding.control = binding.control();
  _payload.binding.shift   = binding.shift();

  if (pressed) {
    _type = Apsis::Engine::Event::Type::Press;
  }
  else {
    _type = Apsis::Engine::Event::Type::Release;
  }
}
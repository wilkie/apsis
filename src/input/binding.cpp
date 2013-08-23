#include "apsis/input/binding.h"

#include <stdlib.h>
#include <string.h>

typedef std::map<std::string, Apsis::Key::Code>::value_type TranslationPair;

static TranslationPair pairs[] = {
  TranslationPair("f1", Apsis::Key::F1),
  TranslationPair("f2", Apsis::Key::F2),
  TranslationPair("f3", Apsis::Key::F3),
  TranslationPair("f4", Apsis::Key::F4),
  TranslationPair("f5", Apsis::Key::F5),
  TranslationPair("f6", Apsis::Key::F6),
  TranslationPair("f7", Apsis::Key::F7),
  TranslationPair("f8", Apsis::Key::F8),
  TranslationPair("f9", Apsis::Key::F9),
  TranslationPair("f10", Apsis::Key::F10),
  TranslationPair("f11", Apsis::Key::F11),
  TranslationPair("f12", Apsis::Key::F12),

  TranslationPair("0", Apsis::Key::NUM0),
  TranslationPair("1", Apsis::Key::NUM1),
  TranslationPair("2", Apsis::Key::NUM2),
  TranslationPair("3", Apsis::Key::NUM3),
  TranslationPair("4", Apsis::Key::NUM4),
  TranslationPair("5", Apsis::Key::NUM5),
  TranslationPair("6", Apsis::Key::NUM6),
  TranslationPair("7", Apsis::Key::NUM7),
  TranslationPair("8", Apsis::Key::NUM8),
  TranslationPair("9", Apsis::Key::NUM9),

  TranslationPair("left", Apsis::Key::LEFT),
  TranslationPair("right", Apsis::Key::RIGHT),
  TranslationPair("up", Apsis::Key::UP),
  TranslationPair("down", Apsis::Key::DOWN),

  TranslationPair("home", Apsis::Key::HOME),
  TranslationPair("page up", Apsis::Key::PAGEUP),
  TranslationPair("page down", Apsis::Key::PAGEDOWN),
  TranslationPair("delete", Apsis::Key::DELETE),
  TranslationPair("insert", Apsis::Key::INSERT),
  TranslationPair("end", Apsis::Key::END),

  TranslationPair("left shift", Apsis::Key::LEFTSHIFT),
  TranslationPair("right shift", Apsis::Key::RIGHTSHIFT),
  TranslationPair("left control", Apsis::Key::LEFTCONTROL),
  TranslationPair("right control", Apsis::Key::RIGHTCONTROL),
  TranslationPair("left alt", Apsis::Key::LEFTALT),
  TranslationPair("right alt", Apsis::Key::RIGHTALT),

  TranslationPair("comma", Apsis::Key::COMMA),
  TranslationPair("period", Apsis::Key::PERIOD),
  TranslationPair("minus", Apsis::Key::MINUS),
  TranslationPair("equals", Apsis::Key::EQUALS),
  TranslationPair("plus", Apsis::Key::PLUS),

  TranslationPair("a", Apsis::Key::A),
  TranslationPair("b", Apsis::Key::B),
  TranslationPair("c", Apsis::Key::C),
  TranslationPair("d", Apsis::Key::D),
  TranslationPair("e", Apsis::Key::E),
  TranslationPair("f", Apsis::Key::F),
  TranslationPair("g", Apsis::Key::G),
  TranslationPair("h", Apsis::Key::H),
  TranslationPair("i", Apsis::Key::I),
  TranslationPair("j", Apsis::Key::J),
  TranslationPair("k", Apsis::Key::K),
  TranslationPair("l", Apsis::Key::L),
  TranslationPair("m", Apsis::Key::M),
  TranslationPair("n", Apsis::Key::N),
  TranslationPair("o", Apsis::Key::O),
  TranslationPair("p", Apsis::Key::P),
  TranslationPair("q", Apsis::Key::Q),
  TranslationPair("r", Apsis::Key::R),
  TranslationPair("s", Apsis::Key::S),
  TranslationPair("t", Apsis::Key::T),
  TranslationPair("u", Apsis::Key::U),
  TranslationPair("v", Apsis::Key::V),
  TranslationPair("w", Apsis::Key::W),
  TranslationPair("x", Apsis::Key::X),
  TranslationPair("y", Apsis::Key::Y),
  TranslationPair("z", Apsis::Key::Z),

  TranslationPair("mouse 0", Apsis::Key::MOUSE_0),
  TranslationPair("mouse 1", Apsis::Key::MOUSE_1),
  TranslationPair("mouse 2", Apsis::Key::MOUSE_2),
  TranslationPair("mouse 3", Apsis::Key::MOUSE_3),
  TranslationPair("mouse 4", Apsis::Key::MOUSE_4),

  TranslationPair("joy 0", Apsis::Key::JOY_0),
  TranslationPair("joy 1", Apsis::Key::JOY_1),
  TranslationPair("joy 2", Apsis::Key::JOY_2),
  TranslationPair("joy 3", Apsis::Key::JOY_3),
  TranslationPair("joy 4", Apsis::Key::JOY_4),
  TranslationPair("joy 5", Apsis::Key::JOY_5),
  TranslationPair("joy 6", Apsis::Key::JOY_6),
  TranslationPair("joy 7", Apsis::Key::JOY_7),
  TranslationPair("joy 8", Apsis::Key::JOY_8),
  TranslationPair("joy 9", Apsis::Key::JOY_9),
  TranslationPair("joy 10", Apsis::Key::JOY_10),
  TranslationPair("joy 11", Apsis::Key::JOY_11),
  TranslationPair("joy 12", Apsis::Key::JOY_12),
  TranslationPair("joy 13", Apsis::Key::JOY_13),
  TranslationPair("joy 14", Apsis::Key::JOY_14),
  TranslationPair("joy 15", Apsis::Key::JOY_15),
  TranslationPair("joy 16", Apsis::Key::JOY_16),
  TranslationPair("joy 17", Apsis::Key::JOY_17),
  TranslationPair("joy 18", Apsis::Key::JOY_18),
  TranslationPair("joy 19", Apsis::Key::JOY_19),

  TranslationPair("joy up", Apsis::Key::JOY_UP),
  TranslationPair("joy down", Apsis::Key::JOY_DOWN),
  TranslationPair("joy left", Apsis::Key::JOY_LEFT),
  TranslationPair("joy right", Apsis::Key::JOY_RIGHT),

  TranslationPair("joy pov left", Apsis::Key::JOY_POV_LEFT),
  TranslationPair("joy pov right", Apsis::Key::JOY_POV_RIGHT),
  TranslationPair("joy pov down", Apsis::Key::JOY_POV_DOWN),
  TranslationPair("joy pov up", Apsis::Key::JOY_POV_UP),
  TranslationPair("joy pov up left", Apsis::Key::JOY_POV_UP_LEFT),
  TranslationPair("joy pov up right", Apsis::Key::JOY_POV_UP_RIGHT),
  TranslationPair("joy pov down left", Apsis::Key::JOY_POV_DOWN_LEFT),
  TranslationPair("joy pov down right", Apsis::Key::JOY_POV_DOWN_RIGHT),
};

static const unsigned int binding_count = sizeof(pairs) / sizeof(TranslationPair);

std::map<std::string, Apsis::Key::Code> Apsis::Input::Binding::_translator(pairs, pairs + binding_count);

Apsis::Input::Binding::Binding(Apsis::Key::Code key,
                               bool shift,
                               bool control,
                               bool alt)
  : _key(key),
    _shift(shift),
    _control(control),
    _alt(alt) {
}

Apsis::Input::Binding::Binding(const char* key,
                               bool shift,
                               bool control,
                               bool alt)
  : _key(_translator[std::string(key)]),
    _shift(shift),
    _control(control),
    _alt(alt) {
}

Apsis::Key::Code Apsis::Input::Binding::key() const {
  return _key;
}

void Apsis::Input::Binding::key(Apsis::Key::Code value) {
  _key = value;
}

bool Apsis::Input::Binding::shift() const {
  return _shift;
}

void Apsis::Input::Binding::shift(bool value) {
  _shift = value;
}

bool Apsis::Input::Binding::alt() const {
  return _alt;
}

void Apsis::Input::Binding::alt(bool value) {
  _alt = value;
}

bool Apsis::Input::Binding::control() const {
  return _control;
}

void Apsis::Input::Binding::control(bool value) {
  _control = value;
}
#ifndef APSIS_INPUT_BINDING_H
#define APSIS_INPUT_BINDING_H

#include "apsis/input/key.h"

#include <string>
#include <map>

namespace Apsis {
  namespace Input {
    class Binding {
    public:
      /*
       *  Constructs a representation of a key binding.
       */
      Binding(Apsis::Key::Code key,
              bool shift,
              bool control,
              bool alt);

      /*
       *  Constructs a representation of a key binding using a string
       *  to represent the key name.
       */
      Binding(const char* key,
              bool shift,
              bool control,
              bool alt);

      /*
       *  Returns the key attached to this binding.
       */
      Apsis::Key::Code key() const;

      /*
       *  Returns whether or not the shift key is held.
       */
      bool shift() const;

      /*
       *  Returns whether or not the control key is held.
       */
      bool control() const;

      /*
       *  Returns whether or not the alt key is held.
       */
      bool alt() const;

      /*
       *  Sets the key code.
       */
      void key(Apsis::Key::Code value);

      /*
       *  Sets whether or not shift is held.
       */
      void shift(bool value);

      /*
       *  Sets whether or not control is held.
       */
      void control(bool value);

      /*
       *  Sets whether or not alt is held.
       */
      void alt(bool value);
    private:
      // Translates key strings to key codes
      static std::map<std::string, Apsis::Key::Code> _translator;

      Apsis::Key::Code _key;

      bool _shift;
      bool _control;
      bool _alt;
    };
  }
}

#endif

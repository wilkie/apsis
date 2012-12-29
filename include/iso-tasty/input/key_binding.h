#ifndef APSIS_KEY_BINDING
#define APSIS_KEY_BINDING

#include "iso-tasty/input/key.h"

namespace Apsis {
  struct KeyBinding {
    Key::Code key;

    bool shift;
    bool control;
    bool alt;
  };
}

#endif

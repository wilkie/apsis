#ifndef ISOTASTY_KEY_BINDING
#define ISOTASTY_KEY_BINDING

#include "iso-tasty/key.h"

namespace IsoTasty {
  struct KeyBinding {
    Key::Code key;

    bool shift;
    bool control;
    bool alt;
  };
}

#endif

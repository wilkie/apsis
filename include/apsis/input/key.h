#ifndef APSIS_KEY_H
#define APSIS_KEY_H

#undef DELETE

namespace Apsis {
  namespace Key {
    enum Code {
      F1,
      F2,
      F3,
      F4,
      F5,
      F6,
      F7,
      F8,
      F9,
      F10,
      F11,
      F12,

      NUM0,
      NUM1,
      NUM2,
      NUM3,
      NUM4,
      NUM5,
      NUM6,
      NUM7,
      NUM8,
      NUM9,

      LEFT,
      RIGHT,
      DOWN,
      UP,

      HOME,
      PAGEUP,
      PAGEDOWN,
      END,

      DELETE,
      INSERT,

      PAUSE,

      LEFTSHIFT,
      RIGHTSHIFT,

      LEFTCONTROL,
      RIGHTCONTROL,

      LEFTALT,
      RIGHTALT,

      COMMA,
      PERIOD,

      MINUS,
      EQUALS,

      PLUS,

      A,
      B,
      C,
      D,
      E,
      F,
      G,
      H,
      I,
      J,
      K,
      L,
      M,
      N,
      O,
      P,
      Q,
      R,
      S,
      T,
      U,
      V,
      W,
      X,
      Y,
      Z,

      MOUSE_0,
      MOUSE_1,
      MOUSE_2,
      MOUSE_3,
      MOUSE_4,

      JOY_0,
      JOY_1,
      JOY_2,
      JOY_3,
      JOY_4,
      JOY_5,
      JOY_6,
      JOY_7,
      JOY_8,
      JOY_9,
      JOY_10,
      JOY_11,
      JOY_12,
      JOY_13,
      JOY_14,
      JOY_15,
      JOY_16,
      JOY_17,
      JOY_18,
      JOY_19,

      JOY_UP,
      JOY_DOWN,
      JOY_LEFT,
      JOY_RIGHT,

      JOY_POV_UP         = 0x1001, // 0001
      JOY_POV_DOWN       = 0x1002, // 0010
      JOY_POV_LEFT       = 0x1004, // 0100
      JOY_POV_RIGHT      = 0x1008, // 1000
      JOY_POV_UP_LEFT    = 0x1005, // 0101
      JOY_POV_UP_RIGHT   = 0x1009, // 1001
      JOY_POV_DOWN_LEFT  = 0x1006, // 0110
      JOY_POV_DOWN_RIGHT = 0x100A, // 1010

      MAX
    };
  }
}

#endif

#ifndef ISOTASTY_MODEL_ANIMATION_H
#define ISOTASTY_MODEL_ANIMATION_H

#include "iso-tasty/sync/reference_counter.h"

namespace IsoTasty {
  namespace Model {
    class Animation {
    public:
      Animation();
      ~Animation();

    private:
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif
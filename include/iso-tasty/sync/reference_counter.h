#ifndef ISOTASTY_SYNC_REFERENCE_COUNTER_H
#define ISOTASTY_SYNC_REFERENCE_COUNTER_H

#include "iso-tasty/sync/atomic_counter.h"

namespace IsoTasty {
  namespace Sync {
    class ReferenceCounter {
    public:
      /*
       *  Constructs a reference counter that keeps track of how many times it
       *    is copied and destroyed.
       */
      ReferenceCounter();
      ReferenceCounter(const ReferenceCounter&);
      ~ReferenceCounter();

      /*
       *  Are we the only one?
       */
      bool isAlone() const;

      /*
       *  Gives the value of the counter.
       */
      unsigned int value() const;

    private:
      // Do not allow this; it isn't useful in the intended usecase anyway.
      ReferenceCounter& operator= (const ReferenceCounter&);

      AtomicCounter* _counter;
    };
  }
}

#endif
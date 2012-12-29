#ifndef APSIS_SYNC_REFERENCE_COUNTER_H
#define APSIS_SYNC_REFERENCE_COUNTER_H

#include "iso-tasty/sync/atomic_counter.h"

namespace Apsis {
  namespace Sync {
    class ReferenceCounter {
    public:
      /*
       *  Constructs a reference counter that keeps track of how many times it
       *    is copied and destroyed.
       */
      ReferenceCounter();
      ReferenceCounter(const ReferenceCounter&);
      ReferenceCounter& operator= (const ReferenceCounter&);
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

      AtomicCounter* _counter;
    };
  }
}

#endif
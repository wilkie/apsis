#ifndef APSIS_SYNC_ATOMIC_COUNTER_H
#define APSIS_SYNC_ATOMIC_COUNTER_H

namespace Apsis {
  namespace Sync {
    class AtomicCounter {
    public:
      /*
       *  Constructs a 32-bit unsigned counter whose operations are guaranteed to be atomic.
       */
      AtomicCounter();
      AtomicCounter(unsigned int value);

      /*
       *  Atomically increments the counter.
       */
      unsigned int increment();

      /*
       *  Atomically decrements the counter.
       */
      unsigned int decrement();

      /*
       *  Adds the given amount to the counter.
       */
      unsigned int add(unsigned int value);

      /*
       *  Gives the value of the counter.
       */
      unsigned int value() const;

    private:
      static bool _compareExchange(unsigned int* reference, unsigned int compare, unsigned int exchange);
      unsigned int _value;
    };
  }
}

#endif

#ifndef APSIS_CLOCK_H
#define APSIS_CLOCK_H

namespace Apsis {
  /*
   *  Maintains a stopwatch that will return the elapsed time since the
   *    previous call.
   */
  class Clock {
    public:
      /*
       *  Constructs and initializes a new Clock.
       */
      Clock();

      /*
       *  Returns: The fraction of seconds since the last call to elapsedTime.
       */
      float elapsedTime();

    private:
      // Internal storage; depends on platform
      void* _internal;
  };
}

#endif

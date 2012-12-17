#ifndef ISOTASTY_CLOCK_H
#define ISOTASTY_CLOCK_H

namespace IsoTasty {
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
      double elapsedTime();

    private:
      // Internal storage; depends on platform
      void* _internal;
  };
}

#endif

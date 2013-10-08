#include "apsis/sync/clock.h"

#ifdef _WIN32
  #include <windows.h>
#elif defined(__linux) || defined(JS_MODE)
  #include <sys/time.h>
#endif

#include <string.h>

namespace Apsis {
  struct ClockPlatformInternal {
#ifdef _WIN32
    LARGE_INTEGER _freq, _start, _end;
#elif defined(__linux) || defined(JS_MODE)
    timeval _now;
    timeval _last;
#endif
  };
}

Apsis::Clock::Clock() {
  ClockPlatformInternal* clock = new ClockPlatformInternal;
  _internal = (void*)clock;
#ifdef _WIN32
  QueryPerformanceFrequency(&clock->_freq);
  QueryPerformanceCounter(&clock->_start);
#elif defined(__linux) || defined(JS_MODE)
  gettimeofday(&clock->_now, NULL);
#endif
}

#if defined(__linux) || defined(JS_MODE)
static float _gettimeofday_difference(timeval *b, timeval *a) {
  timeval diff_tv;

  diff_tv.tv_usec = b->tv_usec - a->tv_usec;
  diff_tv.tv_sec = b->tv_sec - a->tv_sec;

  if (a->tv_usec > b->tv_usec) {
    diff_tv.tv_usec += 1000000;
    diff_tv.tv_sec--;
  }

  return (float)diff_tv.tv_sec + ((float)diff_tv.tv_usec / 1000000.0f);
}
#endif

float Apsis::Clock::elapsedTime() {
  ClockPlatformInternal* clock = (ClockPlatformInternal*)_internal;
#ifdef _WIN32
  QueryPerformanceCounter(&clock->_end);
  LONGLONG amt = (clock->_end.QuadPart - clock->_start.QuadPart) * 1000 / clock->_freq.QuadPart;
  clock->_start = clock->_end;
  return (float)amt / 1000.0f;
#elif defined(__linux) || defined(JS_MODE)
  clock->_last = clock->_now;
  gettimeofday(&clock->_now, NULL);
  return _gettimeofday_difference(&clock->_now, &clock->_last);
#endif
}

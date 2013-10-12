#include "apsis/engine/log.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>

static void _backtrace() {
}
#elif defined(__linux)
  #include <execinfo.h>
  #include <dlfcn.h>
  #include <cxxabi.h>

/*
 * Thanks to Farooq Mela for the helpful code!
 * Very glad to not have to figure out backtrace libraries.
 *
 * Original: https://gist.github.com/fmela/591333
 */
static void _backtrace(int skip = 1) {
  void *callstack[128];
  const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
  char buf[1024];
  int nFrames = backtrace(callstack, nMaxFrames);
  char **symbols = backtrace_symbols(callstack, nFrames);

  for (int i = skip; i < nFrames; i++) {
    Dl_info info;

    if (dladdr(callstack[i], &info) && info.dli_sname) {
      char *demangled = NULL;
      int status = -1;

      if (info.dli_sname[0] == '_') {
        demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
      }

      snprintf(buf, sizeof(buf),
               "%-3d %*p %s + %zd\n",
               i, int(2 + sizeof(void*) * 2), callstack[i],
               status == 0 ? demangled :
               info.dli_sname == 0 ? symbols[i] : info.dli_sname,
               (char *)callstack[i] - (char *)info.dli_saddr);

      free(demangled);
    } else {
      snprintf(buf, sizeof(buf), "%-3d %*p %s!!\n",
          i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
    }
    printf("%s", buf);
  }

  free(symbols);

  if (nFrames == nMaxFrames) {
    printf("[truncated]\n");
  }
}
#endif

void Apsis::Engine::Log::error(const char* src_namespace,
                               const char* src_class,
                               const char* src_function,
                               const char* message) {
  static char foo[1024];

#ifdef _WIN32
  _snprintf(
#else
  snprintf(
#endif
    foo,  1024, "%s::%s#%s Error: %s", src_namespace,
                                                 src_class,
                                                 src_function,
                                                 message);

  printf("%s\n", foo);

  _backtrace();

  throw foo;
}

void Apsis::Engine::Log::printf(const char* fmt, ...) {
#ifdef __linux
  va_list args;
  va_start(args, fmt);
  
  vprintf(fmt, args);
#elif defined(_WIN32)
  va_list args;
  va_start(args, fmt);

  char str[1024];
  vsnprintf(str, 1024, fmt, args);
  OutputDebugStringA(str);
#endif
}
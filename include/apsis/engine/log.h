#ifndef APSIS_ENGINE_LOG_H
#define APSIS_ENGINE_LOG_H

namespace Apsis {
  namespace Engine {
    class Log {
    public:
      static void error(const char* src_namespace,
                        const char* src_class,
                        const char* src_function,
                        const char* message);
      static void warning();
      static void printf(const char* fmt, ...);

    private:
    };
  }
}

#endif

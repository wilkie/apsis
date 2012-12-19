#ifndef ISOTASTY_PRIMITIVES_PROGRAM_H
#define ISOTASTY_PRIMITIVES_PROGRAM_H

#include "iso-tasty/sync/reference_counter.h"

namespace IsoTasty {
  namespace Primitives {
    class Program {
    public:
      /*
       *  Links the given unlinked program.
       */
      Program(unsigned int program);
      ~Program();

      /*
       *  Gives the identifier for this program.
       */
      unsigned int identifer() const;

    private:
      unsigned int _program;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif
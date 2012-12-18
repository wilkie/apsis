#ifndef ISOTASTY_PRIMITIVES_PROGRAM_H
#define ISOTASTY_PRIMITIVES_PROGRAM_H

namespace IsoTasty {
  namespace Primitives {
    class Program {
    public:
      /*
       *  Links the given unlinked program.
       */
      Program(unsigned int program);
      Program(const Program&);
      Program& operator= (const Program&);
      ~Program();

      /*
       *  Gives the identifier for this program.
       */
      unsigned int identifer() const;

    private:
      unsigned int _program;
      unsigned int* _countRef;
    };
  }
}

#endif
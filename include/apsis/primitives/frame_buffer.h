#ifndef APSIS_PRIMITIVES_FRAME_BUFFER_H
#define APSIS_PRIMITIVES_FRAME_BUFFER_H

#include "apsis/sync/reference_counter.h"

namespace Apsis {
  namespace Primitives {
    class FrameBuffer {
    public:
      /*
       *  Constructs a frame buffer object.
       */
      FrameBuffer(unsigned int width, unsigned int height);
      ~FrameBuffer();

      /*
       *  Gives the identifier for this frame buffer.
       */
      unsigned int identifier() const;

    private:
      unsigned int _fbo;
      Sync::ReferenceCounter _counter;

      unsigned int _width;
      unsigned int _height;

      unsigned int _color;
      unsigned int _depth;
      unsigned int _stencil;
      unsigned int _packed;
    };
  }
}

#endif
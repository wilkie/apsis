#ifndef APSIS_APSIS_H
#define APSIS_APSIS_H

// Infrastructure
#include "apsis/input/input_engine.h"
#include "apsis/sync/clock.h"

#include "apsis/viewport.h"
#include "apsis/world/terrain.h"

#include "apsis/backend/sdl.h"

// This is here instead of the cpp because main() is overriden by
// preprocessor magicks.
#ifndef NO_SDL
  #include <SDL/sdl.h>
#endif

namespace Apsis {
  class Engine_ {
  public:
    Engine_(Apsis::Settings::Video& video);
    void newMap(unsigned int width, unsigned int height);
    void run();

  private:
    InputEngine* _input;

    Viewport* _view;
    Renderer* _renderer;
    Apsis::Settings::Video _video;
    Clock* _clock;

    // Backend... can be swapped out for something else
    Apsis::Backend::Sdl _backend;

    void _fireEvent(int event);

    void _draw();
    void _update();
  };
}

#endif
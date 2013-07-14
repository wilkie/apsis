#ifndef APSIS_APSIS_H
#define APSIS_APSIS_H

// Infrastructure
#include "apsis/input/input_engine.h"
#include "apsis/sync/clock.h"

#include "apsis/viewport.h"
#include "apsis/world/terrain.h"

// This is here instead of the cpp because main() is overriden by
// preprocessor magicks.
#ifndef NO_SDL
  #include <SDL/sdl.h>
#endif

namespace Apsis {
  struct VideoSettings {
    unsigned int resolutionX;
    unsigned int resolutionY;
  };

  class Engine {
  public:
    Engine(VideoSettings* video);
    void newMap(unsigned int width, unsigned int height);
    void run();

  private:
    InputEngine* _input;

    Viewport* _view;
    Renderer* _renderer;
    VideoSettings _video;
    Clock* _clock;

    bool _startSDL();
    bool _initialize();
    void _initViewport();
    void _fireEvent(int event);

    void _draw();
    void _update();
  };
}

#endif
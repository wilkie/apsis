#ifndef APSIS_APSIS_H
#define APSIS_APSIS_H

// Infrastructure
#include "iso-tasty/input/input_engine.h"
#include "iso-tasty/sync/clock.h"

#include "iso-tasty/viewport.h"
#include "iso-tasty/map.h"

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
    Map* _map;
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
#ifndef APSIS_BACKEND_SDL_H
#define APSIS_BACKEND_SDL_H

#include <apsis/engine/input.h>
#include <apsis/engine/event.h>

#include <apsis/world/map.h>
#include <apsis/settings/video.h>

// This is here instead of the cpp because main() is overriden by
// preprocessor magicks.
#ifndef NO_SDL
#ifdef JS_MODE
#include <SDL/SDL.h>
#elif defined(SDL1)
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#else
#include <SDL/SDL.h>
#include "SDL2/SDL_mixer.h"
#endif
#endif

// Include GLEW on desktops
#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

namespace Apsis {
  namespace Backend {
    /*
     *  Abstracts sdl related backend stuff.
     */
    class Sdl {
      public:
        /*
         *  Construct an SDL backend. Initialize the backend with the given
         *  video settings. Throws an exception if the backend cannot
         *  initialize.
         */
        Sdl(const Apsis::Settings::Video& video);

        /*
         *  Poll for events. Give a preallocated Event structure. Returns true
         *    when an event is posted.
         */
        bool poll(Apsis::Engine::Event& event);

        /*
         *  Swap buffers.
         */
        void swap();

        /*
         *  Quits.
         */
        void quit();

      private:

        // Internal storage
        Apsis::Settings::Video _video;

        // Initialization methods
        bool _start();
        bool _initialize();

        // Called to draw a frame
        void _draw();

        // Called to update all game objects
        void _update();
    };
  }
}

#endif

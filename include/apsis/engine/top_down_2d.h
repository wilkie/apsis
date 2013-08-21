#ifndef APSIS_ENGINE_TOP_DOWN_2D_H
#define APSIS_ENGINE_TOP_DOWN_2D_H

#include <apsis/input/input_engine.h>
#include <apsis/world/map.h>
#include <apsis/backend/sdl.h>
#include <apsis/settings/video.h>

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/animation.h"
#include "apsis/sprite/thing.h"
#include "apsis/world/actor.h"
#include "apsis/interface/numbers.h"
#include "apsis/interface/filler_bar.h"
#include "apsis/world/background.h"
#include "apsis/world/scene.h"

// This is here instead of the cpp because main() is overriden by
// preprocessor magicks.
#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

namespace Apsis {
  namespace Engine {
    /*
     *  Handles rendering and most major logic.
     */
    class TopDown2d {
      public:
        /*
         *  Construct an Engine initialized with the given display configuration
         */
        TopDown2d(Apsis::Settings::Video& video);

        /*
         *  Execute the game loop. There is no escape from this function until the game ends.
         */
        void run();

      private:

        // Internal storage
        Apsis::Settings::Video _video;
        Apsis::InputEngine* _input;

        Apsis::World::Map* _map;
        unsigned int _player;
        unsigned int _ball;
        Apsis::World::Scene _scene;
        Apsis::World::Background* _bg;

        Apsis::Hud::Numbers* _numbers;
        Apsis::Hud::FillerBar* _health;

        // Backend... can be swapped out for something else
        Apsis::Backend::Sdl _backend;

        // Called to draw a frame
        void _draw();

        // Called to update all game objects
        void _update(float elapsed);

        float _zoom;
        float _x;
        float _z;

        void _fireEvent(int event);
    };
  }
}

#endif
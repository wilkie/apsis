#include "apsis/engine/top_down_2d.h"
#include "apsis/sync/clock.h"

#include "apsis/input/key.h"

#include "math.h"
/*
Engine
  
  Apsis::Event event;

  Apsis::Clock clock;

  while(true) {
    if (backend.poll(event)) {
      game_event = input_engine.post(event);

      handle_game_event(game_event);
    }

    double elapsed = clock.elapsedTime();

    world.update(elapsed);

    foreach actor in actors {
      actor.update(elasped);
    }

    world.draw();

    foreach actor in actors {
      actor.draw();
    }
  }
  */
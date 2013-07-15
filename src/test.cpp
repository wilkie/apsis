#include "apsis.h"
#include "apsis/world/actor.h"

#define WIDTH 1536
#define HEIGHT 864

#include <Windows.h>

int main(int argc, char** argv) {
  // initialize Apsis

  Apsis::Settings::Video settings;
  settings.resolutionX = WIDTH;
  settings.resolutionY = HEIGHT;
  try {
    Apsis::Engine* engine = new Apsis::Engine(settings);
    //Apsis::World::Actor* player1 = new Apsis::World::Actor("assets/actors/herr_von_speck.actor", 250, 175);
    engine->run();
  }
  catch (char* msg) {
    OutputDebugStringA("*** Exception ***\n");
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");
    OutputDebugStringA("*** Exception ***\n");
  }

  return 0;
}
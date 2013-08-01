#include "apsis.h"
#include "apsis/world/actor.h"
#include "apsis/engine/top_down_2d.h"

#define WIDTH 1536
#define HEIGHT 864

#ifdef _WIN32
  #include <Windows.h>
#endif

int main(int argc, char** argv) {
  // initialize Apsis

  Apsis::Settings::Video settings;
  settings.resolutionX = WIDTH;
  settings.resolutionY = HEIGHT;
  try {
    Apsis::Engine::TopDown2d* engine = new Apsis::Engine::TopDown2d(settings);
    engine->run();
  }
  catch (char* msg) {
#ifdef _WIN32
    OutputDebugStringA("*** Exception ***\n");
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");
    OutputDebugStringA("*** Exception ***\n");
#else
    printf("*** Exception ***\n");
    printf(msg);
    printf("\n");
    printf("*** Exception ***\n");
#endif
  }

  return 0;
}

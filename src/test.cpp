#include "apsis.h"
#include "apsis/world/actor.h"
#include "apsis/engine/system.h"

#define WIDTH 1536
#define HEIGHT 864

#ifdef _WIN32
  #include <Windows.h>
#endif

#include <clew/clew.h>

#include "apsis/geometry/line.h"
#include "apsis/geometry/rectangle.h"

int main(int argc, char** argv) {
  // initialize Apsis
  if (clewInit("OpenCL.dll") != CLEW_SUCCESS) {
    //OutputDebugStringA("error opening opencl\n");
  }
  else {
    cl_platform_id test[5];
    cl_uint num;
    cl_uint ok = 5;
    if (clGetPlatformIDs(ok, test, &num) == CL_SUCCESS) {
      char foo[1024];
      sprintf(foo, "%d %d %d %d %d %d\n", test[0],test[1],test[2],test[3],test[4],num);
      //OutputDebugStringA(foo);
      char info[1024];
      size_t size;
      clGetPlatformInfo(test[0], CL_PLATFORM_NAME, 1023, info, &size);
      //OutputDebugStringA(info);
    }
  }

  Apsis::Settings::Video videoSettings;
  Apsis::Settings::Audio audioSettings;
  videoSettings.resolutionX = WIDTH;
  videoSettings.resolutionY = HEIGHT;
  try {
    Apsis::Engine::System system("assets/games/platformer.json",
                                 videoSettings,
                                 audioSettings);

    system.run();

  /* This is the cleaning up part */
  Mix_CloseAudio();
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
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

#include <SDL/SDL_mixer.h>

/* Mix_Music actually holds the music information.  */
Mix_Music *music = NULL;
void musicDone();

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
    //Apsis::Engine::TopDown2d* engine = new Apsis::Engine::TopDown2d(settings);
    //engine->run();

    Apsis::Engine::System system("assets/games/platformer.json",
                                 videoSettings,
                                 audioSettings);
    
  /* We're going to be requesting certain things from our audio
     device, so we set them up beforehand */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  /* This is where we open up our audio device.  Mix_OpenAudio takes
     as its parameters the audio format we'd /like/ to have. */
  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    printf("Unable to open audio!\n");
    exit(1);
  }

  /* If we actually care about what we got, we can ask here.  In this
     program we don't, but I'm showing the function call here anyway
     in case we'd want to know later. */
  Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);

  /* Actually loads up the music */
  music = Mix_LoadMUS("assets/music/stabilizer_piraterap.ogg");

  /* This begins playing the music - the first argument is a
      pointer to Mix_Music structure, and the second is how many
      times you want it to loop (use -1 for infinite, and 0 to
      have it just play once) */
  Mix_PlayMusic(music, 0);

  /* We want to know when our music has stopped playing so we
      can free it up and set 'music' back to NULL.  SDL_Mixer
      provides us with a callback routine we can use to do
      exactly that */
  Mix_HookMusicFinished(musicDone);


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



void handleKey(SDL_KeyboardEvent key);
void musicDone();

int mainfoo(void) {

  SDL_Surface *screen;
  SDL_Event event;
  int done = 0;

  /* We're going to be requesting certain things from our audio
     device, so we set them up beforehand */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  /* This is where we open up our audio device.  Mix_OpenAudio takes
     as its parameters the audio format we'd /like/ to have. */
  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    printf("Unable to open audio!\n");
    exit(1);
  }

  /* If we actually care about what we got, we can ask here.  In this
     program we don't, but I'm showing the function call here anyway
     in case we'd want to know later. */
  Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);

  /* We're going to be using a window onscreen to register keypresses
     in.  We don't really care what it has in it, since we're not
     doing graphics, so we'll just throw something up there. */
  screen = SDL_SetVideoMode(320, 240, 0, 0);

  while(!done) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
	done = 1;
	break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
	handleKey(event.key);
	break;
      }
    }

    /* So we don't hog the CPU */
    SDL_Delay(50);

  }

  /* This is the cleaning up part */
  Mix_CloseAudio();
  SDL_Quit();

  return 0;
}

void handleKey(SDL_KeyboardEvent key) {
  switch(key.keysym.sym) {
  case SDLK_m:
    if(key.state == SDL_PRESSED) {

      /* Here we're going to have the 'm' key toggle the music on and
	 off.  When it's on, it'll be loaded and 'music' will point to
	 something valid.  If it's off, music will be NULL. */

      if(music == NULL) {
	
	/* Actually loads up the music */
	music = Mix_LoadMUS("music.ogg");

	/* This begins playing the music - the first argument is a
	   pointer to Mix_Music structure, and the second is how many
	   times you want it to loop (use -1 for infinite, and 0 to
	   have it just play once) */
	Mix_PlayMusic(music, 0);

	/* We want to know when our music has stopped playing so we
	   can free it up and set 'music' back to NULL.  SDL_Mixer
	   provides us with a callback routine we can use to do
	   exactly that */
	Mix_HookMusicFinished(musicDone);
	
      } else {
	/* Stop the music from playing */
	Mix_HaltMusic();

	/* Unload the music from memory, since we don't need it
	   anymore */
	Mix_FreeMusic(music);
	
	music = NULL;
      }
      break;
    }
  }
}

/* This is the function that we told SDL_Mixer to call when the music
   was finished. In our case, we're going to simply unload the music
   as though the player wanted it stopped.  In other applications, a
   different music file might be loaded and played. */
void musicDone() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  music = NULL;
}
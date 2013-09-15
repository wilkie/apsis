#ifndef APSIS_ENGINE_AUDIO_H
#define APSIS_ENGINE_AUDIO_H

#include "apsis/settings/audio.h"

#include <vector>

/*

Music is generally auto looped... so we need a Audio::Sound class or something.

namespace Audio {
  class Sound {
  public:
    load from JSON
    either a collection of sound files loaded together and played as a loop or
    a single file with metadata dividing it up.

    START LOOP END

    sounds should react to events



  private:
  };
}

{
  "type": "sound",

  "name": "foo",

  "file": "foo.ogg",

  "parts": [
    {
      "tag": "start",
      "length": 10.5
    },
    {
      "tag": "loop",
      "length": 125.5
    },
    {
      "tag": "battle"
    }
  ],

  "arrangements": {
    "normal": ["start, "loop"],
    "transition_to_battle": ["loop", "battle"]
  },

  "playback": {
    "start": "start",
    "loop": "loop",
    "end": "end"
  }
}

*/

#include "apsis/audio/music.h"
#include "apsis/audio/sound.h"

namespace Apsis {
  namespace Engine {
    /*
     *  This class represents the collective state of the audio processor.
     */
    class Audio {
    public:
      static Apsis::Engine::Audio& basic(const Apsis::Settings::Audio& settings);

      /*
       *  Retrieves the current audio settings.
       */
      const Settings::Audio& settings() const;

      /*
       *  Plays given Music.
       */
      void playMusic(const Apsis::Audio::Music& music);

      /*
       *  Plays given Sound.
       */
      void playSound(const Apsis::Audio::Sound& sound);

    private:
      // Constructor
      Audio(const Apsis::Settings::Audio& settings);

      // Keep global track of all Audio Engines
      static std::vector<Apsis::Engine::Audio*> _audio_engines;

      // Audio Settings
      Apsis::Settings::Audio _audioSettings;

      // Current music track
      const Apsis::Audio::Music* _music;
    };
  }
}

#endif
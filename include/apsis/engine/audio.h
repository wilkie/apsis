#ifndef APSIS_ENGINE_AUDIO_H
#define APSIS_ENGINE_AUDIO_H

#include "apsis/settings/audio.h"

#include <vector>

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

    private:
      // Constructor
      Audio(const Apsis::Settings::Audio& settings);

      // Keep global track of all Audio Engines
      static std::vector<Apsis::Engine::Audio*> _audio_engines;

      // Audio Settings
      Apsis::Settings::Audio _audioSettings;
    };
  }
}

#endif
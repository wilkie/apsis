#ifndef APSIS_AUDIO_SOUND_H
#define APSIS_AUDIO_SOUND_H

#include <string>
#include <map>

namespace Apsis {
  namespace Audio {
    class Sound {
    public:
      /*
       *  Creates a representation of the given sound data or loads an
       *  existing one.
       */
      static const Apsis::Audio::Sound& load(const char* path);

      /*
       *  Retrieves the sound data for the given identifier.
       */
      static const Apsis::Audio::Sound& get(size_t identifier);

      /*
       *  Returns a unique identifier for this music data.
       */
      size_t identifier() const;

    private:
      // Constructor
      Sound(const char* path);

      // Keep global track of all sounds.
      static std::map<std::string, size_t> _ids;
      static std::map<size_t, Apsis::Audio::Sound*> _sounds;

      // Path
      std::string _path;

      // Internal data (and identifier)
      void* _sound;
    };
  }
}

#endif
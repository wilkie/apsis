#ifndef APSIS_AUDIO_MUSIC_H
#define APSIS_AUDIO_MUSIC_H

#include <string>
#include <map>

namespace Apsis {
  namespace Audio {
    class Music {
    public:
      /*
       *  Creates a representation of the given music data or loads an
       *  existing one.
       */
      static const Apsis::Audio::Music& load(const char* path);

      /*
       *  Retrieves the music data for the given identifier.
       */
      static const Apsis::Audio::Music& get(size_t identifier);

      /*
       *  Returns a unique identifier for this music data.
       */
      size_t identifier() const;

    private:
      // Constructor
      Music(const char* path);

      // Keep global track of all musics
      static std::map<std::string, size_t> _ids;
      static std::map<size_t, Apsis::Audio::Music*> _musics;

      // Path
      std::string _path;

      // Internal data (and identifier)
      void* _music;
    };
  }
}

#endif
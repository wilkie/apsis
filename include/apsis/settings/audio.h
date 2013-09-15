#ifndef APSIS_SETTINGS_AUDIO_H
#define APSIS_SETTINGS_AUDIO_H

namespace Apsis {
  namespace Settings {
    /*
     *  Describes the audio configuration the Engine should attempt to use.
     */
    struct Audio {
      float soundVolume;
      float musicVolume;
    };
  }
}

#endif
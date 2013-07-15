#ifndef APSIS_SETTINGS_VIDEO_H
#define APSIS_SETTINGS_VIDEO_H

namespace Apsis {
  namespace Settings {
    /*
     *  Describes the display configuration the Engine should attempt to use.
     */
    struct Video {
      /*
       *  The horizontal resolution.
       */
      unsigned int resolutionX;

      /*
       *  The vertical resolution.
       */
      unsigned int resolutionY;
    };
  }
}

#endif
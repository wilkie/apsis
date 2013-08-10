#ifndef APSIS_SPRITE_ANIMATION_H
#define APSIS_SPRITE_ANIMATION_H

#include <string>
#include <vector>

namespace Apsis {
  namespace Sprite {
    class Animation {
    public:
      struct Frame {
        unsigned int spriteIndex;
      };

      Animation(const char* name);

      void addFrame(unsigned int spriteIndex);

      const char* name() const;

      const Apsis::Sprite::Animation::Frame& frame(unsigned int index) const;

      unsigned int count() const;

    private:
      std::string _name;
      std::vector<Frame> _frames;
    };
  }
}

#endif
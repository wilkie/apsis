#include "apsis/sprite/animation.h"

Apsis::Sprite::Animation::Animation(const char* name) {
  _name = std::string(name);
}

void Apsis::Sprite::Animation::addFrame(unsigned int spriteIndex) {
  Frame frame;
  frame.spriteIndex = spriteIndex;
  frame.foo = 12345;

  _frames.push_back(frame);
}

const char* Apsis::Sprite::Animation::name() const {
  return _name.c_str();
}

unsigned int Apsis::Sprite::Animation::count() const {
  return _frames.size();
}

const Apsis::Sprite::Animation::Frame& Apsis::Sprite::Animation::frame(unsigned int index) const {
  return _frames.at(index);
}
#include "apsis/audio/sound.h"

#include "SDL/SDL_mixer.h"

std::map<std::string, size_t> Apsis::Audio::Sound::_ids;
std::map<size_t, Apsis::Audio::Sound*> Apsis::Audio::Sound::_sounds;

Apsis::Audio::Sound::Sound(const char* path)
  : _path(path) {
  _sound = (void*)Mix_LoadWAV(path);
}

const Apsis::Audio::Sound& Apsis::Audio::Sound::get(size_t id) {
  return *_sounds[id];
}

const Apsis::Audio::Sound& Apsis::Audio::Sound::load(const char* path) {
  std::string str = std::string(path);

  if (_ids.count(str) > 0) {
    // already exists
    return *_sounds[_ids[str]];
  }

  Apsis::Audio::Sound* sound = new Apsis::Audio::Sound(path);
  _sounds[sound->identifier()] = sound;
  _ids[str] = sound->identifier();
  return *sound;
}

size_t Apsis::Audio::Sound::identifier() const {
  return (size_t)_sound;
}
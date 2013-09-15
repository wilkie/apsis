#include "apsis/audio/music.h"

#include "SDL/SDL_mixer.h"

std::map<std::string, size_t> Apsis::Audio::Music::_ids;
std::map<size_t, Apsis::Audio::Music*> Apsis::Audio::Music::_musics;

Apsis::Audio::Music::Music(const char* path)
  : _path(path) {
  _music = (void*)Mix_LoadMUS(path);
}

const Apsis::Audio::Music& Apsis::Audio::Music::get(size_t id) {
  return *_musics[id];
}

const Apsis::Audio::Music& Apsis::Audio::Music::load(const char* path) {
  std::string str = std::string(path);

  if (_ids.count(str) > 0) {
    // already exists
    return *_musics[_ids[str]];
  }

  Apsis::Audio::Music* music = new Apsis::Audio::Music(path);
  _musics[music->identifier()] = music;
  _ids[str] = music->identifier();
  return *music;
}

size_t Apsis::Audio::Music::identifier() const {
  return (size_t)_music;
}
#include "apsis/engine/audio.h"

// TODO: Move to backend
#include <SDL/SDL_mixer.h>

std::vector<Apsis::Engine::Audio*> Apsis::Engine::Audio::_audio_engines;

Apsis::Engine::Audio::Audio(const Apsis::Settings::Audio& settings)
  : _audioSettings(settings) {

  // TODO: Use audio settings to set output parameters
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    throw "Engine::Audio Error: Cannot open audio device.";
  }

  // TODO: Use query to determine available audio device parameters.
  // TODO: Place in backend and negotiate.
  // Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
}

Apsis::Engine::Audio& Apsis::Engine::Audio::basic(const Apsis::Settings::Audio& settings) {
  Apsis::Engine::Audio* ae = new Apsis::Engine::Audio(settings);
  _audio_engines.push_back(ae);
  return *ae;
}

const Apsis::Settings::Audio& Apsis::Engine::Audio::settings() const {
  return _audioSettings;
}

void Apsis::Engine::Audio::playMusic(const Apsis::Audio::Music& music) {
  _music = &music;

  Mix_Music* foo = (Mix_Music*)_music->identifier();

  Mix_PlayMusic(foo, 0);
}

void Apsis::Engine::Audio::playSound(const Apsis::Audio::Sound& sound) {
  Mix_Chunk* foo = (Mix_Chunk*)sound.identifier();

  // TODO: Keep track of sound channels.

  Mix_PlayChannel(-1, foo, -1);
}
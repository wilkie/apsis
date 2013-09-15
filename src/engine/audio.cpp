#include "apsis/engine/audio.h"

std::vector<Apsis::Engine::Audio*> Apsis::Engine::Audio::_audio_engines;

Apsis::Engine::Audio::Audio(const Apsis::Settings::Audio& settings)
  : _audioSettings(settings) {
}

Apsis::Engine::Audio& Apsis::Engine::Audio::basic(const Apsis::Settings::Audio& settings) {
  Apsis::Engine::Audio* ae = new Apsis::Engine::Audio(settings);
  _audio_engines.push_back(ae);
  return *ae;
}

const Apsis::Settings::Audio& Apsis::Engine::Audio::settings() const {
  return _audioSettings;
}
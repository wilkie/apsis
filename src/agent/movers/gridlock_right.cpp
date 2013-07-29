#include "apsis/agent/movers/gridlock_right.h"

Apsis::Agent::Movers::GridlockRight::GridlockRight(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move right locked along a grid") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::GridlockRight::update(float elapsed,
                                                std::set<unsigned int>& states,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (states.count(Apsis::State::MOVING_LEFT) +
      states.count(Apsis::State::MOVING_UP)   +
      states.count(Apsis::State::MOVING_DOWN) > 0) {
    return false;
  }

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_RIGHT)) {
    if (states.count(Apsis::State::MOVING_RIGHT) == 0) {
      states.insert(Apsis::State::MOVING_RIGHT);
    }
    updated.x += elapsed * 96.0f;

    return true;
  }
  else if (states.count(Apsis::State::MOVING_RIGHT) != 0) {
    updated.x += elapsed * 96.0f;

    float from_grid_0 = floor(original.x / 32.0f);
    float from_grid_1 = floor(updated.x  / 32.0f);
    
    if (from_grid_1 > from_grid_0) {
      updated.x = from_grid_1 * 32.0f;
      states.erase(Apsis::State::MOVING_RIGHT);
    }

    return true;
  }

  return false;
}
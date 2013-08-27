#include "apsis/agent/movers/gridlock_right.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

Apsis::Agent::Movers::GridlockRight::GridlockRight(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move right locked along a grid") {
  _inputEngine = &inputEngine;

  _rightAction = Apsis::Registry::Action::id("right");

  _movingLeftState  = Apsis::Registry::State::id("movingLeft");
  _movingUpState    = Apsis::Registry::State::id("movingUp");
  _movingRightState = Apsis::Registry::State::id("movingRight");
  _movingDownState  = Apsis::Registry::State::id("movingDown");
}

bool Apsis::Agent::Movers::GridlockRight::update(float elapsed,
                                                Apsis::World::Object& object,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (object.isEnabled(_movingLeftState) ||
      object.isEnabled(_movingUpState)   ||
      object.isEnabled(_movingDownState)) {
    return false;
  }

  if (_inputEngine->isEventHeld(_rightAction)) {
    if (!object.isEnabled(_movingRightState)) {
      object.enableState(_movingRightState);
    }
    updated.x += elapsed * 96.0f;

    return true;
  }
  else if (object.isEnabled(_movingRightState)) {
    updated.x += elapsed * 96.0f;

    float from_grid_0 = floor((float)original.x / 32.0f);
    float from_grid_1 = floor((float)updated.x  / 32.0f);
    
    if (from_grid_1 > from_grid_0) {
      updated.x = from_grid_1 * 32.0f;
      object.disableState(_movingRightState);
    }

    return true;
  }

  return false;
}
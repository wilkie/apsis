#include "apsis/agent/movers/gridlock_up.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

Apsis::Agent::Movers::GridlockUp::GridlockUp(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move up locked along a grid") {
  _inputEngine = &inputEngine;

  _movingLeftState  = Apsis::Registry::State::id("movingLeft");
  _movingUpState    = Apsis::Registry::State::id("movingUp");
  _movingRightState = Apsis::Registry::State::id("movingRight");
  _movingDownState  = Apsis::Registry::State::id("movingDown");

  _upAction = Apsis::Registry::Action::id("up");
}

bool Apsis::Agent::Movers::GridlockUp::update(float elapsed,
                                                Apsis::World::Object& object,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (object.isEnabled(_movingLeftState)  ||
      object.isEnabled(_movingRightState) ||
      object.isEnabled(_movingDownState)) {
    return false;
  }

  if (_inputEngine->isEventHeld(_upAction)) {
    if (!object.isEnabled(_movingUpState)) {
      object.enableState(_movingUpState);
    }
    updated.y -= elapsed * 96.0f;

    return true;
  }
  else if (object.isEnabled(_movingUpState)) {
    updated.y -= elapsed * 96.0f;

    float from_grid_0 = floor((float)original.y / 32.0f);
    float from_grid_1 = floor((float)updated.y  / 32.0f);
    
    if (from_grid_1 < from_grid_0) {
      updated.y = from_grid_0 * 32.0f;
      object.disableState(_movingUpState);
    }

    return true;
  }

  return false;
}
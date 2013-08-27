#include "apsis/agent/movers/jump.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

Apsis::Agent::Movers::Jump::Jump(Apsis::InputEngine& inputEngine,
                                 float maximumHeight,
                                 float startingVelocity,
                                 float deacceleration,
                                 float peakDeacceleration,
                                 float minimumVelocity)
  : _maximumHeight(maximumHeight),
    _startingVelocity(startingVelocity),
    _deacceleration(deacceleration),
    _peakDeacceleration(peakDeacceleration),
    _minimumVelocity(minimumVelocity),
    Apsis::Agent::Mover("jump") {
  _inputEngine = &inputEngine;

  _collideWithLeftState   = Apsis::Registry::State::id("collideWithLeft");
  _collideWithRightState  = Apsis::Registry::State::id("collideWithRight");
  _collideWithTopState  = Apsis::Registry::State::id("collideWithTop");
  _collideWithBottomState  = Apsis::Registry::State::id("collideWithBottom");

  _jumpingState  = Apsis::Registry::State::id("jumping");
  _canJumpState  = Apsis::Registry::State::id("canJump");

  _jumpAction = Apsis::Registry::Action::id("jump");
}

bool Apsis::Agent::Movers::Jump::update(float elapsed,
                                        Apsis::World::Object& object,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (object.isEnabled(_collideWithTopState) &&
      !object.isEnabled(_jumpingState)) {
    // If we detect we are landed, allow jump.
    _velocity = _startingVelocity;
    _height = 0.0f;
    if (!_inputEngine->isEventHeld(_jumpAction)) {
      object.enableState(_canJumpState);
    }
  }
  
  if (object.isEnabled(_collideWithBottomState)) {
    // Release jump upon hitting a ceiling.
    object.disableState(_jumpingState);
    object.disableState(_canJumpState);
    object.disableState(_collideWithBottomState);
    return false;
  }
  else if (!object.isEnabled(_canJumpState) &&
           !object.isEnabled(_jumpingState)) {
    // Do not allow double jumps.
  }
  else if (_inputEngine->isEventHeld(_jumpAction) &&
           object.isEnabled(_canJumpState)) {
    // While key is held, jump until maximum height.
    object.enableState(_jumpingState);
    object.disableState(_collideWithTopState);

    float amount = elapsed * _velocity;

    _height += amount;
    if (_height > _maximumHeight) {
      // Do not allow a jump after our maximum height has been reached.
      amount = _maximumHeight - _height;
      object.disableState(_canJumpState);
    }

    updated.y -= amount;

    _velocity -= _deacceleration * elapsed;
    if (_velocity < _minimumVelocity) {
      _velocity = _minimumVelocity;
    }
    return true;
  }
  else if (object.isEnabled(_jumpingState)) {
    // Released key, done jumping, deaccelerate.
    object.disableState(_canJumpState);

    if (_velocity > 0) {
      _velocity -= _peakDeacceleration * elapsed;
      updated.y -= elapsed * _velocity;
      return true;
    }
    else {
      object.disableState(_jumpingState);
      _velocity = _startingVelocity;
      _height = 0.0f;
    }
  }
  else {
    _velocity = _startingVelocity;
    _height = 0.0f;
  }

  return false;
}
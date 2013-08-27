#include "apsis/agent/movers/wall_jump.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

Apsis::Agent::Movers::WallJump::WallJump(Apsis::InputEngine& inputEngine,
                                         float maximumHeight,
                                         float startingVelocity,
                                         float deacceleration,
                                         float minimumVelocity,
                                         float startingHorizontalVelocity,
                                         float horizontalDeacceleration)
  : _maximumHeight(maximumHeight),
    _startingVelocity(startingVelocity),
    _deacceleration(deacceleration),
    _minimumVelocity(minimumVelocity),
    _startingHorizontalVelocity(startingHorizontalVelocity),
    _horizontalDeacceleration(horizontalDeacceleration),
    Apsis::Agent::Mover("wall jump") {
  _inputEngine = &inputEngine;

  // Rules this supercedes
  supercede("jump");

  _collideWithLeftState   = Apsis::Registry::State::id("collideWithLeft");
  _collideWithRightState  = Apsis::Registry::State::id("collideWithRight");
  _collideWithTopState  = Apsis::Registry::State::id("collideWithTop");
  _collideWithBottomState  = Apsis::Registry::State::id("collideWithBottom");

  _jumpingState  = Apsis::Registry::State::id("jumping");
  _canJumpState  = Apsis::Registry::State::id("canJump");

  _jumpAction = Apsis::Registry::Action::id("jump");
}

bool Apsis::Agent::Movers::WallJump::update(float elapsed,
                                            Apsis::World::Object& object,
                                            const Apsis::Geometry::Rectangle& original,
                                            Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if ((object.isEnabled(_collideWithLeftState) ||
       object.isEnabled(_collideWithRightState)) &&
      !object.isEnabled(_collideWithTopState) &&
      !object.isEnabled(_jumpingState)) {
    // If we detect we are on a wall, but not landed, allow jump.
    _velocity = _startingVelocity;

    _horizontalVelocity = _startingHorizontalVelocity;

    _height = 0.0f;
    _wallJumping = true;
    
    _goingRight = object.isEnabled(_collideWithLeftState);

    if (!_inputEngine->isEventHeld(_jumpAction)) {
      object.enableState(_canJumpState);
    }
  }
  else if (!object.isEnabled(_jumpingState)) {
    _wallJumping = false;
  }
  
  if (_wallJumping) {
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
      object.disableState(_collideWithLeftState);
      object.disableState(_collideWithRightState);
      object.disableState(_collideWithTopState);

      float amount = elapsed * _velocity;

      _height += amount;
      if (_height > _maximumHeight) {
        // Do not allow a jump after our maximum height has been reached.
        amount = _maximumHeight - _height;
        object.disableState(_canJumpState);
        object.disableState(_jumpingState);
      }

      updated.y -= amount;

      _velocity -= _deacceleration * elapsed;
      if (_velocity < _minimumVelocity) {
        _velocity = _minimumVelocity;
      }

      // Stop moving upward when we cannot move sideways.
      _horizontalVelocity -= _horizontalDeacceleration * elapsed;
      if (_horizontalVelocity <= 0) {
        _horizontalVelocity = 0;
        object.disableState(_canJumpState);
        object.disableState(_jumpingState);
      }
      if (_goingRight) {
        updated.x += _horizontalVelocity * elapsed;
      }
      else {
        updated.x -= _horizontalVelocity * elapsed;
      }

      return true;
    }
    else if (object.isEnabled(_jumpingState)) {
      // Released key, done jumping.
      object.disableState(_canJumpState);
    }
    else {
      _velocity = _startingVelocity;
      _height = 0.0f;
    }
  }

  return false;
}
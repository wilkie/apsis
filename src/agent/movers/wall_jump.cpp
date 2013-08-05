#include "apsis/agent/movers/wall_jump.h"

#include "apsis/registry/state.h"

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
}

bool Apsis::Agent::Movers::WallJump::update(float elapsed,
                                            std::set<unsigned int>& states,
                                            const Apsis::Geometry::Rectangle& original,
                                            Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if ((states.count(_collideWithLeftState) > 0 ||
       states.count(_collideWithRightState) > 0) &&
       states.count(_collideWithTopState) == 0 &&
      states.count(_jumpingState) == 0) {
    // If we detect we are on a wall, but not landed, allow jump.
    _velocity = _startingVelocity;

    _horizontalVelocity = _startingHorizontalVelocity;

    _height = 0.0f;
    _wallJumping = true;
    
    _goingRight = states.count(_collideWithLeftState) > 0;

    if (!_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_JUMP)) {
      states.insert(_canJumpState);
    }
  }
  else if (states.count(_jumpingState) == 0) {
    _wallJumping = false;
  }
  
  if (_wallJumping) {
    if (states.count(_collideWithBottomState) > 0) {
      // Release jump upon hitting a ceiling.
      states.erase(_jumpingState);
      states.erase(_canJumpState);
      states.erase(_collideWithBottomState);
      return false;
    }
    else if (states.count(_canJumpState) == 0 &&
             states.count(_jumpingState) == 0) {
      // Do not allow double jumps.
    }
    else if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_JUMP) &&
             states.count(_canJumpState) > 0) {
      // While key is held, jump until maximum height.
      states.insert(_jumpingState);
      states.erase(_collideWithLeftState);
      states.erase(_collideWithRightState);
      states.erase(_collideWithTopState);

      float amount = elapsed * _velocity;

      _height += amount;
      if (_height > _maximumHeight) {
        // Do not allow a jump after our maximum height has been reached.
        amount = _maximumHeight - _height;
        states.erase(_canJumpState);
        states.erase(_jumpingState);
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
        states.erase(_canJumpState);
        states.erase(_jumpingState);
      }
      if (_goingRight) {
        updated.x += _horizontalVelocity * elapsed;
      }
      else {
        updated.x -= _horizontalVelocity * elapsed;
      }

      return true;
    }
    else if (states.count(_jumpingState) > 0) {
      // Released key, done jumping.
      states.erase(_canJumpState);
    }
    else {
      _velocity = _startingVelocity;
      _height = 0.0f;
    }
  }

  return false;
}
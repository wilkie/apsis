#ifndef APSIS_WORLD_ACTOR_H
#define APSIS_WORLD_ACTOR_H

// Actors: interact with worlds
// Sprite: 2d actor
//  Thing: 3d actor

// Actors have agents that decide how they affect the world, how world affects
// them.

// MapCollider - Collides with Map walls, impassible blocks
// SimplePhysics2d - Simple physics
// Physics2d   - 2d physics

// Agents are given a new position and an old position and some properties and
// decide where the actor actually moves and fires events.

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/animation.h"
#include "apsis/sprite/thing.h"

#include "apsis/agent/impeder.h"
#include "apsis/agent/mover.h"
#include "apsis/geometry/rectangle.h"
#include "apsis/world/object.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include "apsis/registry/property.h"

#include "apsis/sync/reference_counter.h"

#include <vector>
#include <set>

namespace Apsis {
  namespace World {
    class Actor {
    public:
      /*
       *  Constructs a World::Actor from a Thing.
       */
      Actor(const Apsis::Sprite::Thing& thing,
            unsigned int x,
            unsigned int y);

      /*
       *  Return: the Apsis::Sprite::Sheet for the Apsis::Sprite.
       */
      const Apsis::Sprite::Sheet& sheet() const;

      /*
       *  Return: the Apsis::Geometry::Rectangle for the Apsis::Sprite.
       */
      Apsis::Geometry::Rectangle position() const;

      /*
       *  Sets the current animation to be played by this Apsis::Sprite.
       */
      void animate(const char* animationName);

      /*
       *  Advances the frame to the next sprite.
       */
      void nextFrame();

      /*
       *  Updates the current time for the Sprite. Affects animations and movements.
       */
      void update(float elapsed);

      void move(Apsis::Geometry::Point& to);

      // Add the given agent that will provide movement to the actor.
      void attachMover(Apsis::Agent::Mover* agent);

      // Add the given agent that will alter intended movement.
      void attachImpeder(Apsis::Agent::Impeder* agent);

      // Draws the actor
      void draw(const glm::mat4& projection,
                Primitives::Camera& camera) const;

      // List rules
      const char* rules() const;

    private:
      // The base Thing for this Actor.
      const Apsis::Sprite::Thing& _thing;

      // The set of sprites for the Actor.
      const Apsis::Sprite::Sheet& _sheet;

      // Stores the current animation.
      const Sprite::Animation* _animation;

      // Stores the current frame.
      unsigned int _currentFrame;
      const Sprite::Animation::Frame* _frame;

      // The Object composition of this Actor.
      Apsis::World::Object _object;

      // The current position of the Actor in the world.
      Apsis::Geometry::Rectangle _position;

      // Stores the current state of the character. State
      // determines how the character updates.
      const char* _state;

      // time since last frame.
      float _currentTime;

      // Before Move Agents
      std::vector<Apsis::Agent::Impeder*> _impederAgents;
      std::vector<Apsis::Agent::Mover*>   _moverAgents;
    };
  }
}

#endif

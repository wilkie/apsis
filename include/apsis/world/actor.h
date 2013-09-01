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

#include "apsis/rules/collide_function.h"
#include "apsis/rules/update_function.h"

#include "apsis/geometry/rectangle.h"

#include "apsis/world/object.h"
#include "apsis/world/rule_set.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include "apsis/registry/property.h"
#include "apsis/registry/rule.h"

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
      void update(Apsis::World::Scene& scene, float elapsed);

      /*
       *  Reacts to an input action.
       */
      void act(Apsis::World::Scene& scene, unsigned int action_id, bool held);

      /*
       *  Responds to collisions.
       */
      void collide(Apsis::World::Scene& scene);

      /*
       *  Adds the given rule to the actor's rule set.
       */
      void attachRule(const Apsis::Registry::Rule& rule);

      /*
       *  Draws the actor.
       */
      void draw(const glm::mat4& projection,
                const Primitives::Camera& camera) const;

      /*
       *  Returns the name of this Actor.
       */
      const char* name() const;

    private:
      struct UpdaterRule {
        Apsis::Rules::UpdateFunction func;
        unsigned int skipNext;
      };

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

      // Rules
      RuleSet _ruleSet;
    };
  }
}

#endif

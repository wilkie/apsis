#ifndef APSIS_WORLD_RULE_SET_H
#define APSIS_WORLD_RULE_SET_H

#include "apsis/geometry/rectangle.h"

#include "apsis/registry/rule.h"

#include "apsis/world/collision_object.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Scene;
    class Object;

    class RuleSet {
    public:
      /*
       *  Adds the given rule to this rule set.
       */
      void addRule(const Apsis::Registry::Rule& rule);

      /*
       *  Returns the number of rules in this set.
       */
      unsigned int count() const;

      /*
       *  Returns the rule at the given index in this set.
       */
      const Apsis::Registry::Rule& rule(unsigned int id) const;

      /*
       *  Updates the given object from the given scene.
       */
      void update(float elapsed,
                  Apsis::World::Object& object,
                  const Apsis::World::Scene& scene) const;

      /*
       *  Checks collisions of the given object within the given scene.
       */
      bool collide(const Apsis::World::Scene& scene,
                   Apsis::World::Object& object,
                   const Apsis::Geometry::Rectangle& original,
                   const Apsis::Geometry::Point& intended,
                   Apsis::Geometry::Point& clipped) const;

      /*
       *  Updates objects that react to a particular input action.
       */
      void act(unsigned int action_id,
               bool held,
               Apsis::World::Object& object,
               const Apsis::World::Scene& scene) const;

      /*
       *  Responds to the given event.
       */
      void respond(unsigned int event_id,
                   Apsis::World::Object& object,
                   Apsis::World::Scene& scene) const;
    private:
      // Should do this data structure for each type of function.
      struct RuleNode {
        const Apsis::Registry::Rule* rule;
        unsigned int                 next;
      };

      std::vector<RuleNode> _rules;
    };
  }
}

#endif
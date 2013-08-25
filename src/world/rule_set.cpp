#include "apsis/world/rule_set.h"

void Apsis::World::RuleSet::addRule(const Apsis::Registry::Rule& rule) {
  RuleNode foo = {&rule, 0};

  // Look up existing rules and see if this any of these rule supercede the new one
  for (unsigned int i = 0; i < _rules.size(); i++) {
    const Apsis::Registry::Rule& existing_rule = *_rules[i].rule;

    if (existing_rule.supercedes(rule.name())) {
      // existing_rule supercedes rule
      // in this case, rule should come after existing_rule
      // existing_rule's skip is incremented
      _rules[i].next++;
      _rules.insert(_rules.begin() + i + 1, foo);

      return;
    }

    if (rule.supercedes(existing_rule.name())) {
      // rule supercedes existing_rule
      // it should then come before this rule
      // it should be inserted before the rule and
      // have the skip set to 1 more than the existing_rule skip

      foo.next = _rules[i].next;
      _rules.insert(_rules.begin() + i, foo);

      return;
    }
  }

  // Otherwise, foo happens
  _rules.push_back(foo);

  // TODO: what if a situation like the following happens:

  // rules A and B exist
  // rule C supercedes both A and B.

  // We would find A, put C as its parent, and B would not be affected.
  // But alas, B should also be a child of C. So we would have to loop through
  // the rest and continue to reorder (now we reorder with C in place)

  // Generally, rules won't be complicated. We only need to supercede rules for
  // specificity. For instance, a wall slide would supercede the more general
  // 'fall' rule. So, when wall slide is active (only happens when next to a
  // wall in the scene) fall will not trigger. So, that is to say, there is
  // only typically a A -> B -> C relationship between rules... directly
  // linear, not grouped.

  // Nonetheless, it should be supported for those cases that will undoubtly
  // come up.

  // TODO: detect cycles and error out.
}

unsigned int Apsis::World::RuleSet::count() const {
  return _rules.size();
}

const Apsis::Registry::Rule& Apsis::World::RuleSet::rule(unsigned int id) const {
  return *_rules[id].rule;
}

void Apsis::World::RuleSet::update(float elapsed,
                                   Apsis::World::Object& object,
                                   const Apsis::World::Scene& scene) const {
  for(unsigned int i = 0; i < _rules.size(); i++) {
    const RuleNode& node = _rules[i];

    const Apsis::Registry::Rule& rule = *node.rule;

    for (unsigned int rule_id = 0; rule_id < rule.updateFunctionCount(); rule_id++) {
      bool updated = rule.updateFunction(rule_id)(elapsed, scene, 0, object);

      // Skip inner rules
      if (updated) {
        i += node.next;
      }
    }
  }
}

void Apsis::World::RuleSet::act(unsigned int action_id,
                                bool held,
                                Apsis::World::Object& object,
                                const Apsis::World::Scene& scene) const {
  for(unsigned int i = 0; i < _rules.size(); i++) {
    const RuleNode& node = _rules[i];

    const Apsis::Registry::Rule& rule = *node.rule;

    for (unsigned int rule_id = 0; rule_id < rule.actFunctionCount(); rule_id++) {
      if (rule.actionId(rule_id) == action_id) {
        bool updated = rule.actFunction(rule_id)(action_id, held, scene, 0, object);

        // Skip inner rules
        if (updated) {
          i += node.next;
        }
      }
      else {
        // Skip inner rules
        i += node.next;
      }
    }
  }
}

bool Apsis::World::RuleSet::collide(const Apsis::World::Scene& scene,
                                    const Apsis::World::Object& object,
                                    const Apsis::Geometry::Rectangle& original,
                                    const Apsis::Geometry::Point& intended,
                                    Apsis::World::CollisionObject& collidedWith,
                                    Apsis::Geometry::Point& clipped) const {
  bool collided = false;
  clipped = intended;

  for(unsigned int i = 0; i < _rules.size(); i++) {
    const RuleNode& node = _rules[i];

    const Apsis::Registry::Rule& rule = *node.rule;

    for (unsigned int rule_id = 0; rule_id < rule.collideFunctionCount(); rule_id++) {
      Apsis::Geometry::Point clippedTmp = clipped;

      if (rule.collideFunction(rule_id)(scene, 0, object, original, clipped, collidedWith, clippedTmp)) {
        clipped = clippedTmp;
        collided = true;
      }
    }
  }

  return collided;
}
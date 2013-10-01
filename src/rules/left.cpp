#include "apsis/rules/left.h"

#include "apsis/registry/action.h"

bool Apsis::Rules::Left::update(float elapsed,
                                const Apsis::World::Scene& scene,
                                Apsis::World::Object& object) {

  static unsigned int x_id     = Apsis::Registry::Property::id("x");
  static unsigned int speed_id = Apsis::Registry::Property::id("left_speed");
  static unsigned int left_id = Apsis::Registry::State::id("moving_left");

  float x = (float)object.get(x_id).asDouble();

  if (!object.has(speed_id)) {
    object.set(speed_id, 70.0f);
  }

  if (object.isEnabled(left_id)) {
    x -= elapsed * (float)object.get(speed_id).asDouble();
    object.set(x_id, x);
  }

  return false;
}

const char* Apsis::Rules::Left::action = "left";

bool Apsis::Rules::Left::act(unsigned int action_id,
                             bool held,
                             const Apsis::World::Scene& scene,
                             Apsis::World::Object& object) {
  static unsigned int left_action_id = Apsis::Registry::Action::id("left").id();
  static unsigned int left_id = Apsis::Registry::State::id("moving_left");

  if (action_id == left_action_id) {
    if (held) {
      object.enableState(left_id);
    }
    else {
      object.disableState(left_id);
    }
    return true;
  }

  return false;
}
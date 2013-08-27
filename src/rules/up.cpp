#include "apsis/rules/up.h"

#include "apsis/registry/action.h"

bool Apsis::Rules::Up::update(float elapsed,
                              const Apsis::World::Scene& scene,
                              const unsigned int objectId,
                              Apsis::World::Object& object) {

  static unsigned int y_id     = Apsis::Registry::Property::id("y");
  static unsigned int speed_id = Apsis::Registry::Property::id("up_speed");
  static unsigned int up_id = Apsis::Registry::State::id("moving_up");

  float y = (float)object.get(y_id).asDouble();

  if (!object.has(speed_id)) {
    object.set(speed_id, 70.0f);
  }

  if (object.isEnabled(up_id)) {
    y -= elapsed * (float)object.get(speed_id).asDouble();
    object.set(y_id, y);
  }

  return false;
}

const char* Apsis::Rules::Up::action = "up";

bool Apsis::Rules::Up::act(unsigned int action_id,
                           bool held,
                           const Apsis::World::Scene& scene,
                           const unsigned int objectId,
                           Apsis::World::Object& object) {
  static unsigned int up_action_id = Apsis::Registry::Action::id("up").id();
  static unsigned int up_id = Apsis::Registry::State::id("moving_up");

  if (action_id == up_action_id) {
    if (held) {
      object.enableState(up_id);
    }
    else {
      object.disableState(up_id);
    }
    return true;
  }

  return false;
}
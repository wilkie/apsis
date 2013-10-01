#include "apsis/rules/update_health.h"

void Apsis::Rules::UpdateHealth::respond(unsigned int event_id,
                                         const Apsis::World::Scene&,
                                         Apsis::World::Object& object) {
  static unsigned int health_id = Apsis::Registry::Property::id("health");
  static unsigned int amount_id = Apsis::Registry::Property::id("amount");

  float amount = (float)object.getForEvent(event_id, amount_id).asDouble();

  float health = (float)object.get(health_id).asDouble();
  health += amount;

  object.set(health_id, health);
}
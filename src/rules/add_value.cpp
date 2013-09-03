#include "apsis/rules/add_value.h"

void Apsis::Rules::AddValue::respond(unsigned int event_id,
                                     const Apsis::World::Scene&,
                                     const unsigned int objectId,
                                     Apsis::World::Object& object) {
  static unsigned int amount_id   = Apsis::Registry::Property::id("amount");
  static unsigned int property_id = Apsis::Registry::Property::id("property");

  float amount = (float)object.getForEvent(event_id, amount_id).asDouble();
  const char* property = object.getForEvent(event_id, property_id).asCString();

  unsigned int value_id = Apsis::Registry::Property::id(property);

  float value = (float)object.get(value_id).asDouble();
  value += amount;

  object.set(value_id, value);
}
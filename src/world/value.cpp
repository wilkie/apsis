#include <apsis/world/value.h>

Apsis::World::Value::Value() {
  this->_values.asDouble = 0.0;
}

Apsis::World::Value::Value(long value) {
  this->_values.asLong = value;
}

Apsis::World::Value::Value(double value) {
  this->_values.asDouble = value;
}

Apsis::World::Value::Value(const char* value) {
  this->_valueString = std::string(value);
}

double Apsis::World::Value::asDouble() const {
  return this->_values.asDouble;
}

long Apsis::World::Value::asInteger() const {
  return this->_values.asLong;
}

const char* Apsis::World::Value::asCString() const {
  return this->_valueString.c_str();
}
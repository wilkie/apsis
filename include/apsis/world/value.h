#ifndef APSIS_WORLD_VALUE_H
#define APSIS_WORLD_VALUE_H

#include <string>

namespace Apsis {
  namespace World {
    class Value {
    public:
      Value(long value);
      Value(double value);
      Value(const char* value);

      double asDouble();
      long asInteger();
      const char* asCString();

    private:
      union {
        long        asLong;
        double      asDouble;
      } _values;

      std::string _valueString;
    };
  }
}

#endif
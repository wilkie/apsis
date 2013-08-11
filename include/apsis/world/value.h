#ifndef APSIS_WORLD_VALUE_H
#define APSIS_WORLD_VALUE_H

#include <string>

namespace Apsis {
  namespace World {
    class Value {
    public:
      /*
       *  Contruct a value of 0.0.
       */
      Value();

      /*
       *  Construct a Value with an integer.
       */
      Value(long value);

      /*
       *  Construct a Value with a floating point.
       */
      Value(double value);

      /*
       *  Construct a Value with a string.
       */
      Value(const char* value);

      double asDouble() const;
      long asInteger() const;
      const char* asCString() const;

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
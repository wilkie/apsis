#ifndef APSIS_AGENT_RESPONDER_H
#define APSIS_AGENT_RESPONDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include "apsis/world/object.h"

#include <set>

namespace Apsis {
  namespace Agent {
    class Responder {
    public:
      Responder(const char* rule);

      /*
       *  Update intended point and return reason for change.
       */
      virtual bool uponEvent(Apsis::World::Object& object,
                             Apsis::Geometry::Rectangle& original,
                             Apsis::Geometry::Point& intended);

      bool supercedes(const char* rule);

      void supercede(const char* rule);

      const char* rule();
	private:
      std::set<unsigned int> _supercedes;
      const char*            _rule;
      unsigned int           _id;

      static unsigned int registerRule(const char* rule);
      static std::vector<std::string> _ids;
    };
  }
}

#endif

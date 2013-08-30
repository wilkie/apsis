#ifndef APSIS_GEOMETRY_LINE_H
#define APSIS_GEOMETRY_LINE_H

#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Geometry {
    struct Line {
      Point points[2];

      void unit(Line* line);
      void unit(Line* line, float magnitude);

      float magnitude();

      bool intersects(Line* line);
      bool clips(Line* line, Point* point);
    };
  }
}

#endif

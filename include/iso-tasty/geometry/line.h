#ifndef ISOTASTY_GEOMETRY_LINE_H
#define ISOTASTY_GEOMETRY_LINE_H

#include "iso-tasty/geometry/point.h"

namespace IsoTasty {
  namespace Geometry {
    struct Line {
      Point points[2];

      void unit(Line* line);
      void unit(Line* line, double magnitude);

      double magnitude();

      bool intersects(Line* line);
      bool clips(Line* line, Point* point);
    };
  }
}

#endif

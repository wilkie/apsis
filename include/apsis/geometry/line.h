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

      /*
       *  Gives the point starting from the start point along the line
       *  at the given percentage. The range of t is [0.0, 1.0].
       */
      Point at(float t);

      /*
       *  Gives the percentage down the line starting from the start
       *  point the given Point is on the line. The range for the resule
       *  is within [0.0, 1.0].
       */
      float where(Apsis::Geometry::Point& point);

      /*
       *  Returns true when the given line is the same as this line.
       */
      bool equals(Apsis::Geometry::Line& line);

      bool intersects(Line* line);
      bool clips(Line* line, Point* point);
    };
  }
}

#endif

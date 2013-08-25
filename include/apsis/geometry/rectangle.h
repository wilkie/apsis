#ifndef APSIS_GEOMETRY_RECTANGLE_H
#define APSIS_GEOMETRY_RECTANGLE_H

#include "apsis/geometry/line.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Geometry {
    struct Rectangle {
      float x;
      float y;

      float width;
      float height;

      bool intersects(Rectangle* rectangle);
      bool intersects(Line* line);

      unsigned int clip(Line* line, double* tMin, double* tMax);

      bool contains(Point* point);

      bool on(Point* point);
      bool on(Line* line);

      bool corner(Point* point);

      void points(Point points[4]) const;
      void lines(Line lines[4]) const;

      /*
       *  Returns a Line representing the given edge. 0 = Left, 1 = Right,
       *  2 = Top, 3 = Bottom.
       */
      Apsis::Geometry::Line edge(unsigned int index) const;

      Apsis::Geometry::Point center() const;
    };
  }
}

#endif

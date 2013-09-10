#ifndef APSIS_GEOMETRY_RECTANGLE_H
#define APSIS_GEOMETRY_RECTANGLE_H

#include "apsis/geometry/line.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace Geometry {
    struct Rectangle {
      Rectangle(float x, float y, float width, float height);

      float x;
      float y;

      float width;
      float height;

      bool intersects(Rectangle* rectangle) const;
      bool intersects(Line* line) const;

      unsigned int clip(Line* line, float* tMin, float* tMax) const;

      bool contains(Point* point) const;

      bool on(Point* point) const;
      bool on(Line* line) const;

      bool corner(Point* point) const;

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

#ifndef ISOTASTY_GEOMETRY_CIRCLE_H
#define ISOTASTY_GEOMETRY_CIRCLE_H

#include "iso-tasty/geometry/point.h"
#include "iso-tasty/geometry/rectangle.h"
#include "iso-tasty/geometry/line.h"

namespace IsoTasty {
  namespace Geometry {
    struct Circle {
      public:
        Point  position;
        double radius;

        bool intersects(Circle* circle);

        bool intersects(Rectangle* rectangle);
        unsigned int clip(Rectangle* rectangle, Point intersectionPoints[4]);

        bool intersects(Line* line);
        bool clip(Line* line);

        bool contains(Line* line);
        bool contains(Point* point);

        bool on(Point* point);
    };
  }
}

#endif

#ifndef APSIS_GEOMETRY_CIRCLE_H
#define APSIS_GEOMETRY_CIRCLE_H

#include "apsis/geometry/point.h"
#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/line.h"

namespace Apsis {
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

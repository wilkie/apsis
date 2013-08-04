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

      void points(Point points[4]);
      void lines(Line lines[4]);

      Apsis::Geometry::Point center() const;
    };
  }
}

#endif

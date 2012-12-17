#ifndef ISOTASTY_GEOMETRY_RECTANGLE_H
#define ISOTASTY_GEOMETRY_RECTANGLE_H

#include "iso-tasty/geometry/line.h"
#include "iso-tasty/geometry/point.h"

namespace IsoTasty {
  namespace Geometry {
    struct Rectangle {
      double x;
      double y;

      double width;
      double height;

      bool intersects(Rectangle* rectangle);
      bool intersects(Line* line);

  	  bool clip(Line* line, double* tMin, double* tMax);

      bool contains(Point* point);

  	  bool on(Point* point);
  	  bool on(Line* line);

	    bool corner(Point* point);

      void points(Point points[4]);
      void lines(Line lines[4]);
    };
  }
}

#endif

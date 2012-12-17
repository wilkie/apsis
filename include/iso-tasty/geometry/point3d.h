#ifndef ISOTASTY_GEOMETRY_POINT3D_H
#define ISOTASTY_GEOMETRY_POINT3D_H

namespace IsoTasty {
  namespace Geometry {
    struct Point3d {
      // The x, y and z coordinates of the object's footprint in the world.
      double x;
      double y;
      double z;
    };
  }
}

#endif

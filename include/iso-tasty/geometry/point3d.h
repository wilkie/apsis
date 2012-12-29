#ifndef APSIS_GEOMETRY_POINT3D_H
#define APSIS_GEOMETRY_POINT3D_H

namespace Apsis {
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

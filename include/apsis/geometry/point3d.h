#ifndef APSIS_GEOMETRY_POINT3D_H
#define APSIS_GEOMETRY_POINT3D_H

namespace Apsis {
  namespace Geometry {
    struct Point3d {
      // The x, y and z coordinates of the object's footprint in the world.
      float x;
      float y;
      float z;
    };
  }
}

#endif

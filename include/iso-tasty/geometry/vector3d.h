#ifndef ISOTASTY_GEOMETRY_VECTOR3D_H
#define ISOTASTY_GEOMETRY_VECTOR3D_H

namespace IsoTasty {
  namespace Geometry {
    class Vector3d {
    public:
      Vector3d(float x, float y, float z);
      Vector3d(float values[3]);

      float x() const;
      float y() const;
      float z() const;

      float magnitude() const;
      float dotProduct(Vector3d &b) const;
      Vector3d cross(Vector3d &b) const;
      Vector3d scalar(float value) const;
      Vector3d normalize() const;
      Vector3d add(Vector3d &b) const;
      Vector3d subtract(Vector3d &b) const;

    private:
      float _x;
      float _y;
      float _z;
    };
  }
}

#endif
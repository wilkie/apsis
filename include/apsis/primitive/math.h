#ifndef APSIS_PRIMITIVE_MATH_H
#define APSIS_PRIMITIVE_MATH_H

namespace Apsis {
  namespace Primitive {
    class Matrix {
    public:
      float value[4][4];

      static Primitive::Matrix identity();

    private:
    };

    class Vector2 {
    public:
      float value[2];

      float x() const;
      float y() const;
    private:
    };

    class Vector3 {
    public:
      float value[3];

      float x() const;
      float y() const;
      float z() const;
    private:
    };

    class Vector4 {
    public:
      float value[4];

      float x() const;
      float y() const;
      float z() const;
      float w() const;
    };
  }
}

#endif

#include <iso-tasty/geometry/vector3d.h>

#include <math.h>

IsoTasty::Geometry::Vector3d::Vector3d(float x, float y, float z) :
  _x(x), _y(y), _z(z) {
}

IsoTasty::Geometry::Vector3d::Vector3d(float values[3]) :
  _x(values[0]), _y(values[1]), _z(values[2]) {
}

float IsoTasty::Geometry::Vector3d::magnitude() const {
  return sqrtf(_x * _x + _y * _y + _z * _z);
}

float IsoTasty::Geometry::Vector3d::dotProduct(Vector3d &b) const {
  return (_x * b._x) + (_y * b._y) + (_z * b._z);
}

IsoTasty::Geometry::Vector3d IsoTasty::Geometry::Vector3d::cross(Vector3d &b) const {
  return Vector3d((_y * b._z) - (_z * b._y),
                  (_z * b._x) - (_x * b._z),
                  (_x * b._y) - (_y * b._x));
}

IsoTasty::Geometry::Vector3d IsoTasty::Geometry::Vector3d::scalar(float value) const {
  return Vector3d(_x * value, _y * value, _z * value);
}

IsoTasty::Geometry::Vector3d IsoTasty::Geometry::Vector3d::normalize() const {
  float magnitude = this->magnitude();
  return Vector3d(_x / magnitude, _y / magnitude, _z / magnitude);
}

IsoTasty::Geometry::Vector3d IsoTasty::Geometry::Vector3d::add(Vector3d &b) const {
  return Vector3d(_x + b._x, _y + b._y, _z + b._z);
}

IsoTasty::Geometry::Vector3d IsoTasty::Geometry::Vector3d::subtract(Vector3d &b) const {
  return Vector3d(_x - b._x, _y - b._y, _z - b._z);
}

float IsoTasty::Geometry::Vector3d::x() const {
  return _x;
}

float IsoTasty::Geometry::Vector3d::y() const {
  return _y;
}

float IsoTasty::Geometry::Vector3d::z() const {
  return _z;
}
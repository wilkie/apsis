#include "iso-tasty/geometry/line.h"

#include <math.h>

void Apsis::Geometry::Line::unit(Apsis::Geometry::Line* line) {
  unit(line, 1.0);
}

void Apsis::Geometry::Line::unit(Apsis::Geometry::Line* line, double magnitude) {
  double magnitudeX = points[1].x - points[0].x;
  double magnitudeY = points[1].y - points[0].y;

  double magnitudeT = magnitudeX * magnitudeX + magnitudeY * magnitudeY;
  magnitudeT = sqrt(magnitudeT);

  // Find unit magnitudes
  magnitudeX = magnitudeX / magnitudeT;
  magnitudeY = magnitudeY / magnitudeT;

  line->points[0] = points[0];

  line->points[1].x = points[0].x + magnitudeX * magnitude;
  line->points[1].y = points[0].y + magnitudeY * magnitude;
}

double Apsis::Geometry::Line::magnitude() {
  double magnitudeX = points[1].x - points[0].x;
  double magnitudeY = points[1].y - points[0].y;

  double magnitude = magnitudeX * magnitudeX + magnitudeY * magnitudeY;
  magnitude = sqrt(magnitude);

  return magnitude;
}

bool Apsis::Geometry::Line::intersects(Line* line) {
  double x1  = points[0].x;
  double x2  = line->points[0].x;

  double y1  = points[0].y;
  double y2  = line->points[0].y;

  double dX1 = points[1].x - points[0].x;
  double dY1 = points[1].y - points[0].y;

  double dX2 = line->points[1].x - line->points[0].x;
  double dY2 = line->points[1].y - line->points[0].y;

  // Find t in (x1-x2)/(dX2-dX1) = t (for both tX and tY)
  // This formula is derived from the parametric equation for a line

  double tX = (x1 - x2) / (dX2 - dX1);
  double tY = (y1 - y2) / (dY2 - dY1);

  if (dX2 == dX1 && dY2 == dY1) {
    // Parallel
    return false;
  }

  // t defines the intersection point along the line
  // when 't' is within [0, 1], this is a point on the line
  if (tX >= 0.0 && tX <= 1.0 && tY >= 0.0 && tY <= 1.0) {
    return true;
  }

  return false;
}

bool Apsis::Geometry::Line::clips(Line* line, Point* point) {
  double x1, x2, x3, x4;
  double y1, y2, y3, y4;

  x1 = this->points[0].x;
  x2 = this->points[1].x;
  x3 = line->points[0].x;
  x4 = line->points[1].x;

  y1 = this->points[0].y;
  y2 = this->points[1].y;
  y3 = line->points[0].y;
  y4 = line->points[1].y;

  // Find t in (x1-x2)/(dX2-dX1) = t (for both t1 and t2)
  // This formula is derived from the parametric equation for a line

  double t1, t2;

  double determinant = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

  t1  = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
  t1 /= determinant;

  t2  = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);
  t2 /= determinant;

  if (determinant == 0) {
    // Parallel
    return false;
  }

  // t defines the intersection point along the line
  // when 't' is within [0, 1], this is a point on the line
  if (!(t1 >= 0.0 && t1 <= 1.0 && t2 >= 0.0 && t2 <= 1.0)) {
    return false;
  }

  point->x = x1 + t1 * (x2 - x1);
  point->y = y1 + t1 * (y2 - y1);
  return true;
}
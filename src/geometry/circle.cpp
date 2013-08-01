#include "apsis/geometry/circle.h"

#include <stdlib.h>
#include <math.h>

bool Apsis::Geometry::Circle::intersects(Rectangle* rectangle) {
  bool intersects = false;
  Line l;

  double halfWidth  = rectangle->width / 2.0;
  double halfHeight = rectangle->height / 2.0;

  Point points[4];
  points[0].x = rectangle->x - halfWidth;
  points[0].y = rectangle->y - halfHeight;
  points[1].x = rectangle->x - halfWidth;
  points[1].x = rectangle->y + halfHeight;
  points[2].x = rectangle->x + halfWidth;
  points[2].y = rectangle->y + halfHeight;
  points[3].x = rectangle->x + halfWidth;
  points[3].y = rectangle->y - halfHeight;

  l.points[0] = points[0];
  l.points[1] = points[1];
  intersects |= this->intersects(&l);

  l.points[0] = points[1];
  l.points[1] = points[2];
  intersects |= this->intersects(&l);

  l.points[0] = points[2];
  l.points[1] = points[3];
  intersects |= this->intersects(&l);

  l.points[0] = points[3];
  l.points[1] = points[0];
  intersects |= this->intersects(&l);

  return intersects;
}

unsigned int Apsis::Geometry::Circle::clip(Rectangle* rectangle, Point intersectionPoints[4]) {
  unsigned int numberPoints = 0;
  Line l;

  double halfWidth  = rectangle->width / 2.0;
  double halfHeight = rectangle->height / 2.0;

  Point points[4];
  points[0].x = rectangle->x - halfWidth;
  points[0].y = rectangle->y - halfHeight;
  points[1].x = rectangle->x - halfWidth;
  points[1].y = rectangle->y + halfHeight;
  points[2].x = rectangle->x + halfWidth;
  points[2].y = rectangle->y + halfHeight;
  points[3].x = rectangle->x + halfWidth;
  points[3].y = rectangle->y - halfHeight;

  for (unsigned int i = 0; i < 4; i++) {
    l.points[0] = points[i];
    l.points[1] = points[(i+1)%4];
    if (this->clip(&l)) {
      if (l.points[0].x != points[i].x || l.points[0].y != points[i].y) {
        intersectionPoints[numberPoints] = l.points[0];
        numberPoints++;
      }
      if (l.points[1].x != points[(i+1)%4].x || l.points[1].y != points[(i+1)%4].y) {
        if (l.points[1].x != l.points[0].x || l.points[1].y != l.points[0].y) {
          intersectionPoints[numberPoints] = l.points[1];
          numberPoints++;
        }
      }
    }
  }
  if (numberPoints > 4) {
	  numberPoints = numberPoints;
  }

  return numberPoints;
}

bool Apsis::Geometry::Circle::intersects(Line* line) {
  double x1, y1, x2, y2;
  x1 = line->points[0].x - position.x;
  x2 = line->points[1].x - position.x;
  y1 = line->points[0].y - position.y;
  y2 = line->points[1].y - position.y;

  double dx, dy;
  dx = x2 - x1;
  dy = y2 - y1;

  double magnitude = line->magnitude();

  double determinant;
  determinant = x1 * y2 - x2 * y1;

  double discriminant;
  discriminant = radius*radius * magnitude*magnitude - determinant*determinant;

  double sign_of_dy = 1.0;
  if (dy < 0.0) {
    sign_of_dy = -1.0;
  }

  if (discriminant == 0) {
    // Tangent, 1 intersection point
    double x = determinant * dy - sign_of_dy * dx * sqrt(discriminant);
    x /= magnitude*magnitude;
    double y = -determinant * dx - fabs(dy) * sqrt(discriminant);
    y /= magnitude*magnitude;

    Line clipped;
    clipped.points[0].x = x;
    clipped.points[0].y = y;
	clipped.points[1].x = x2;
	clipped.points[1].y = y2;

    double clippedMagnitude = clipped.magnitude();

    clipped.points[1] = clipped.points[0];
    clipped.points[0].x = x1;
    clipped.points[0].y = y1;

    double clippedMagnitude2 = clipped.magnitude();

    if (clippedMagnitude2 > clippedMagnitude) {
      clippedMagnitude = clippedMagnitude2;
    }

    if (clippedMagnitude < magnitude) {
      // We actually clipped something
      return true;
    }
  }
  else if (discriminant > 0) {
    // Secant, 2 intersection points
    double x = determinant * dy - sign_of_dy * dx * sqrt(discriminant);
    x /= magnitude*magnitude;
    double y = -determinant * dx - abs(dy) * sqrt(discriminant);
    y /= magnitude*magnitude;

    Line clipped;
    clipped.points[0].x = x;
    clipped.points[0].y = y;
	clipped.points[1].x = x2;
	clipped.points[1].y = y2;

    double clippedMagnitude = clipped.magnitude();

    if (clippedMagnitude < magnitude) {
      // We actually clipped something!
      return true;
    }

    clipped.points[0].x = x1;
    clipped.points[0].y = y1;

    x  = determinant * dy + sign_of_dy * dx * sqrt(discriminant);
    x /= magnitude*magnitude;
    y  = -determinant * dx + abs(dy) * sqrt(discriminant);
    y /= magnitude*magnitude;

    clipped.points[1].x = x;
    clipped.points[1].y = y;

    clippedMagnitude = clipped.magnitude();
    if (clippedMagnitude < magnitude) {
      // We actually clipped something
      return true;
    }
  }
  else {
    // No intersection
  }

  return false;
}

bool Apsis::Geometry::Circle::clip(Line* line) {
  // Flip line so that points[0] is inside circle
  bool flipped = false;
  if (contains(line)) {
    // No clipping needed
    return false;
  }
  else if (contains(&line->points[1])) {
    // Swap
    Point tmp = line->points[0];
    line->points[0] = line->points[1];
    line->points[1] = tmp;
    flipped = true;
  }
  else if (!contains(&line->points[0])) {
    // Outside of circle
    return false;
  }

  // Relate to origin being center of circle
  line->points[0].x -= position.x;
  line->points[0].y -= position.y;
  line->points[1].x -= position.x;
  line->points[1].y -= position.y;

  double dx, dy;
  dx = line->points[1].x - line->points[0].x;
  dy = line->points[1].y - line->points[0].y;

  double magnitude = line->magnitude();

  double determinant;
  determinant  = line->points[0].x * line->points[1].y;
  determinant -= line->points[1].x * line->points[0].y;

  double discriminant;
  discriminant = radius*radius * magnitude*magnitude - determinant*determinant;

  double sign_of_dy = 1.0;
  if (dy < 0.0) {
    sign_of_dy = -1.0;
  }

  bool hasClipped = false;

  if (discriminant == 0) {
    // Tangent, 1 intersection point
    // line is on circle
    return false;
  }
  else if (discriminant > 0) {
    // Secant, 2 intersection points
    double x1 = determinant * dy - sign_of_dy * dx * sqrt(discriminant);
    x1 /= magnitude*magnitude;
    double y1 = -determinant * dx - abs(dy) * sqrt(discriminant);
    y1 /= magnitude*magnitude;

    double x2 = determinant * dy + sign_of_dy * dx * sqrt(discriminant);
    x2 /= magnitude*magnitude;
    double y2 = -determinant * dx + abs(dy) * sqrt(discriminant);
    y2 /= magnitude*magnitude;

    Line clipped;
    clipped.points[0].x = x1;
    clipped.points[0].y = y1;
    clipped.points[1] = line->points[1];

    double clippedMagnitude = clipped.magnitude();

    clipped.points[0].x = x2;
    clipped.points[0].y = y2;

    double clippedMagnitude2 = clipped.magnitude();

    if (clippedMagnitude2 < clippedMagnitude) {
      // Second point is on line
      line->points[1].x = x2;
      line->points[1].y = y2;
    }
    else {
      // First point is on line
      line->points[1].x = x1;
      line->points[1].y = y1;
    }
    hasClipped = true;
  }
  else {
    // No intersection
  }

  line->points[0].x += position.x;
  line->points[0].y += position.y;
  line->points[1].x += position.x;
  line->points[1].y += position.y;

  if (flipped) {
    // Swap
    Point tmp = line->points[0];
    line->points[0] = line->points[1];
    line->points[1] = tmp;
  }

  return hasClipped;
}

bool Apsis::Geometry::Circle::intersects(Circle* circle) {
  Line l;
  l.points[0] = position;
  l.points[1] = circle->position;
  return l.magnitude() < (radius + circle->radius);
}

bool Apsis::Geometry::Circle::contains(Line* line) {
  return contains(&line->points[0]) && contains(&line->points[1]);
}

bool Apsis::Geometry::Circle::contains(Point* point) {
  double dx = point->x - position.x;
  double dy = point->y - position.y;

  double distance = sqrt(dx * dx + dy * dy);

  return distance <= radius;
}

bool Apsis::Geometry::Circle::on(Point* point) {
  double dx = point->x - position.x;
  double dy = point->y - position.y;

  double distance = sqrt(dx * dx + dy * dy);

  return distance == radius;
}

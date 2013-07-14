#include "iso-tasty/geometry/rectangle.h"

bool Apsis::Geometry::Rectangle::intersects(Rectangle* rectangle) {
  bool doesNotIntersect = false;
  doesNotIntersect |= rectangle->x > (x + width);
  doesNotIntersect |= x            > (rectangle->x + rectangle->width);
  doesNotIntersect |= rectangle->y > (y + height);
  doesNotIntersect |= y            > (rectangle->y + rectangle->height);
  return !doesNotIntersect;
}

bool Apsis::Geometry::Rectangle::intersects(Line* line) {
  double deltaX = (line->points[1].x - line->points[0].x);
  double deltaY = (line->points[1].y - line->points[0].y);

  // Liang-Barsky Algorithm to find the intersection point
 
  // Parametric Equation for a Line:
  // x = boundingBox.x + t * (boundingBox.width);
  // y = boundingBox.y + t * (boundingBox.height);
  
  // Use these with the sides of the rectangle to find values of 't'

  // If these are within the range [0..1], then the line intersects the axis of
  //   the rectangle.
  double tMin, tMax;
  tMin = 0.0;
  tMax = 1.0;

  double p[4];
  double q[4];
  double exactPosition[4];

  double halfWidth  = width / 2.0;
  double halfHeight = height / 2.0;

  double left   = x - halfWidth;
  double right  = x + halfWidth;
  double top    = y - halfWidth;
  double bottom = y + halfWidth;

  // Left
  p[0] = -deltaX;
  q[0] = line->points[0].x - left;
  exactPosition[0] = left;

  // Right
  p[1] = deltaX;
  q[1] = right - line->points[0].x;
  exactPosition[1] = right;

  // Top
  p[2] = -deltaY;
  q[2] = line->points[0].y - top;
  exactPosition[2] = top;

  // Bottom
  p[3] = deltaY;
  q[3] = bottom - line->points[0].y;
  exactPosition[3] = bottom;

  // Special cases (Dare I call them edge cases?)
  // Gliding past the edge
  if (p[0] == 0 && p[1] == 0) {
    if (line->points[0].x == left || line->points[0].x == right) {
      return false;
    }
  }
  else if (p[2] == 0 && p[3] == 0) {
    if (line->points[0].y == top || line->points[0].y == bottom) {
      return false;
    }
  }

  // Unix Vector Magnitudes
  double magnitudeX = deltaX;
  double magnitudeY = deltaY;
  double r;

  for (int i = 0; i < 4; i++) {
    if (p[i] < 0.0) {
      // Outside to Inside
      r = q[i] / p[i];
      if (r > tMax) {
        return false;
      }
      else if (r > tMin) {
        tMin = r;
      }
    }
    else if (p[i] > 0.0) {
      // Inside to Outside
      r = q[i] / p[i];
      if (r < tMin) {
        return false;
      }
      else if (r < tMax) {
        tMax = r;
      }
    }
    else if (p[i] == 0.0 && q[i] < 0.0) {
      // Reject
      return false;
    }
  }

  if (tMin > tMax) {
    // Outside
    return false;
  }

  // Line intersects between tMin and tMax
  return true;
}

bool Apsis::Geometry::Rectangle::clip(Line* line, double* tMin, double* tMax) {
  double deltaX = (line->points[1].x - line->points[0].x);
  double deltaY = (line->points[1].y - line->points[0].y);

  // Liang-Barsky Algorithm to find the intersection point
 
  // Parametric Equation for a Line:
  // x = boundingBox.x + t * (boundingBox.width);
  // y = boundingBox.y + t * (boundingBox.height);
  
  // Use these with the sides of the rectangle to find values of 't'

  // If these are within the range [0..1], then the line intersects the axis of
  //   the rectangle.
  *tMin = 0.0;
  Point  pMin;
  *tMax = 1.0;
  Point  pMax;

  double p[4];
  double q[4];
  double exactPosition[4];

  double halfWidth  = width / 2.0;
  double halfHeight = height / 2.0;

  double left   = x - halfWidth;
  double right  = x + halfWidth;
  double top    = y - halfWidth;
  double bottom = y + halfWidth;

  // Left
  p[0] = -deltaX;
  q[0] = line->points[0].x - left;
  exactPosition[0] = left;

  // Right
  p[1] = deltaX;
  q[1] = right - line->points[0].x;
  exactPosition[1] = right;

  // Top
  p[2] = -deltaY;
  q[2] = line->points[0].y - top;
  exactPosition[2] = top;

  // Bottom
  p[3] = deltaY;
  q[3] = bottom - line->points[0].y;
  exactPosition[3] = bottom;

  // Special cases (Dare I call them edge cases?)
  // Gliding past the edge
  if (p[0] == 0 && p[1] == 0) {
    if (line->points[0].x == left || line->points[0].x == right) {
      return false;
    }
  }
  else if (p[2] == 0 && p[3] == 0) {
    if (line->points[0].y == top || line->points[0].y == bottom) {
      return false;
    }
  }

  // Unix Vector Magnitudes
  double magnitudeX = deltaX;
  double magnitudeY = deltaY;
  double r;

  for (int i = 0; i < 4; i++) {
    if (p[i] < 0.0) {
      // Outside to Inside
      r = q[i] / p[i];
      if (r > *tMax) {
        return false;
    }
    else if (r > *tMin) {
        *tMin = r;
        if (i < 2) {
          pMin.x = exactPosition[i];
          pMin.y = line->points[0].y + *tMin * magnitudeY;
        }
        else {
          pMin.y = exactPosition[i];
          pMin.x = line->points[0].x + *tMin * magnitudeX;
        }
      }
    }
    else if (p[i] > 0.0) {
      // Inside to Outside
      r = q[i] / p[i];
      if (r < *tMin) {
        return false;
      }
      else if (r < *tMax) {
        *tMax = r;
        if (i < 2) {
          pMax.x = exactPosition[i];
          pMax.y = line->points[0].y + *tMax * magnitudeY;
        }
        else {
          pMax.y = exactPosition[i];
          pMax.x = line->points[0].x + *tMax * magnitudeX;
        }
      }
    }
    else if (p[i] == 0.0 && q[i] < 0.0) {
      // Reject
      return false;
    }
  }

  if (*tMin > *tMax) {
    // Outside
    return false;
  }

  // Line intersects between tMin and tMax

  if (*tMin > 0.0) {
    line->points[0] = pMin;
  }
  else {
    *tMin = 0.0;
  }

  if (*tMax < 1.0) {
    line->points[1] = pMax;
  }
  else {
    *tMax = 1.0;
  }

  if (*tMin == 0.0 && *tMax == 1.0) {
    // Inside rectangle
    return true;
  }

  return true;
}

bool Apsis::Geometry::Rectangle::contains(Point* point) {
  double halfWidth  = width  / 2.0;
  double halfHeight = height / 2.0;

  bool containsPoint = true;
  containsPoint &= point->x >= x - halfWidth;
  containsPoint &= point->y >= y - halfHeight;
  containsPoint &= point->x <  x + halfWidth;
  containsPoint &= point->y <  y + halfHeight;
  return containsPoint;
}

bool Apsis::Geometry::Rectangle::on(Point* point) {
  double halfWidth  = width  / 2.0;
  double halfHeight = height / 2.0;

  double left   = x - halfWidth;
  double right  = x + halfWidth;
  double top    = y - halfHeight;
  double bottom = y + halfHeight;

  if (point->x == left && point->y >= top && point->y <= bottom) {
    return true;
  }

  if (point->x == right && point->y >= top && point->y <= bottom) {
    return true;
  }
  
  if (point->y == top && point->x >= left && point->x <= right) {
    return true;
  }
  
  if (point->y == bottom && point->x >= left && point->x <= right) {
    return true;
  }

  return false;
}

bool Apsis::Geometry::Rectangle::corner(Point* point) {
  double halfWidth  = width  / 2.0;
  double halfHeight = height / 2.0;

  double left   = x - halfWidth;
  double right  = x + halfWidth;
  double top    = y - halfHeight;
  double bottom = y + halfHeight;

  if (point->x == left && point->y == top) {
    return true;
  }

  if (point->x == left && point->y == bottom) {
    return true;
  }
  
  if (point->x == right && point->y == top) {
    return true;
  }
  
  if (point->x == right && point->y == bottom) {
    return true;
  }

  return false;
}

bool Apsis::Geometry::Rectangle::on(Line* line) {
  if (line->points[0].x == line->points[1].x || line->points[0].y == line->points[1].y) {
  return this->on(&line->points[0]) && this->on(&line->points[1]);
  }

  return false;
}

void Apsis::Geometry::Rectangle::points(Point points[4]) {
  double halfWidth  = width / 2.0;
  double halfHeight = height / 2.0;

  points[0].x = x - halfWidth;
  points[0].y = y - halfHeight;

  points[1].x = x + halfWidth;
  points[1].y = y - halfHeight;

  points[2].x = x + halfWidth;
  points[2].y = y + halfHeight;

  points[3].x = x - halfWidth;
  points[3].y = y + halfHeight;
}

void Apsis::Geometry::Rectangle::lines(Line lines[4]) {
  Point points[4];
  this->points(points);

  for (int i = 0; i < 4; i++) {
    lines[i].points[0] = points[i];
    lines[i].points[1] = points[(i+1)%4];
  }
}
#ifndef ANIMATION_INTERPOLATION_H
#define ANIMATION_INTERPOLATION_H

#include <linalg/Vec2D.h>

namespace Interpolation {
    double Linear(double t);
    double Bezier(const Vec2D &p1, const Vec2D &p2, double t);
    double Bouncing(double t);

    double dLinear(double t, double dt);
    double dBezier(const Vec2D &p1, const Vec2D &p2, double t, double dt);
    double dBouncing(double t, double dt);
};

#endif //ANIMATION_INTERPOLATION_H

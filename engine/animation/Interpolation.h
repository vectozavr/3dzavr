//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef ENGINE_INTERPOLATION_H
#define ENGINE_INTERPOLATION_H

#include "../math/Vec2D.h"

namespace Interpolation {
    double Linear(double t);

    double Bezier(const Vec2D &p1, const Vec2D &p2, double t);

    double Bouncing(double t);

    double dLinear(double t, double dt);

    double dCos(double t, double dt);

    double dBezier(const Vec2D &p1, const Vec2D &p2, double t, double dt);

    double dBouncing(double t, double dt);
};

#endif //INC_3DZAVR_INTERPOLATION_H

#ifndef ANIMATION_INTERPOLATION_H
#define ANIMATION_INTERPOLATION_H

#include <linalg/Vec2D.h>

namespace Interpolation {
    /**
     * Calculates a linear interpolation between two points.
     *
     * @param t The interpolation parameter, typically in the range [0, 1].
     * @return The interpolated value.
     */
    double Linear(double t);

    /**
     * Calculates a Bezier interpolation using two control points.
     *
     * @param p1 The first control point.
     * @param p2 The second control point.
     * @param t The interpolation parameter, typically in the range [0, 1].
     * @return The interpolated value.
     */
    double Bezier(const Vec2D &p1, const Vec2D &p2, double t);

    /**
     * Calculates a bouncing interpolation effect.
     *
     * @param t The interpolation parameter, typically in the range [0, 1].
     * @return The interpolated value, with a bouncing effect applied.
     */
    double Bouncing(double t);

    /**
     * Calculates the derivative of a linear interpolation.
     *
     * @param t The interpolation parameter.
     * @param dt The change in the interpolation parameter.
     * @return The derivative of the linear interpolation at t.
     */
    double dLinear(double t, double dt);

    /**
     * Calculates the derivative of a Bezier interpolation.
     *
     * @param p1 The first control point.
     * @param p2 The second control point.
     * @param t The interpolation parameter.
     * @param dt The change in the interpolation parameter.
     * @return The derivative of the Bezier interpolation at t.
     */
    double dBezier(const Vec2D &p1, const Vec2D &p2, double t, double dt);

    /**
     * Calculates the derivative of a bouncing interpolation effect.
     *
     * @param t The interpolation parameter.
     * @param dt The change in the interpolation parameter.
     * @return The derivative of the bouncing effect at t.
     */
    double dBouncing(double t, double dt);
};

#endif //ANIMATION_INTERPOLATION_H

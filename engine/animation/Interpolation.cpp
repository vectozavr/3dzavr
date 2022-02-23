//
// Created by Иван Ильин on 09.11.2021.
//
#include <cmath>

#include "Interpolation.h"
#include "../Consts.h"

double Interpolation::Linear(double t) {
    if (t < 0) {
        t = -t;
    }
    int integer = static_cast<int>(t);

    return (integer % 2) ? 1.0 - (t - integer) : (t - integer);
}

double Interpolation::Bezier(const Vec2D &p1, const Vec2D &p2, double t) {
    // TODO: implement bezier curve without finding the root of equation
    t = Interpolation::Linear(t);

    double h = Consts::EPS;
    double eps = Consts::EPS;

    // We are trying to find 's' when px = t
    auto f = [=](double s) {
        return 3.0 * (1.0 - s) * (1.0 - s) * s * p1.x() + 3.0 * (1.0 - s) * s * s * p2.x() + s * s * s - t;
    };
    // Using found 's' we will calculate resulting py
    auto py = [=](double s) {
        return 3.0 * (1.0 - s) * (1.0 - s) * s * p1.y() + 3.0 * (1.0 - s) * s * s * p2.y() + s * s * s;
    };

    auto df = [=](double s) {
        return (f(s + h) - f(s - h)) / (2.0 * h);
    };

    // Newton method
    double s1 = 0.0, s2 = 0.5;
    int i = 0;

    while (std::abs(s1 - s2) > eps) {
        s1 = s2;
        s2 = s1 - f(s1) / df(s1);
        i++;
    }

    return py(s1);
}

double Interpolation::Bouncing(double t) {
    t = Interpolation::Linear(t);
    return 0.5 * (1.0 / (1.0 + exp(10.0 * (-4.0 * t + 0.8))) +
                  (1.0 + 2.5 * sin(50.0 * (t - 1.0 / 3.0)) * exp(-7.0 * t)) / (1.0 + exp(10.0 * (-15.0 * t + 3.1))));
}

double Interpolation::dLinear(double t, double dt) {
    return ((int) trunc(t) % 2) ? -dt : dt;
}

double Interpolation::dCos(double t, double dt) {
    return 0.5 * Consts::PI * sin(Consts::PI * t) * dt;
}

double Interpolation::dBezier(const Vec2D &p1, const Vec2D &p2, double t, double dt) {
    return Interpolation::Bezier(p1, p2, t + dt) - Interpolation::Bezier(p1, p2, t);
}

double Interpolation::dBouncing(double t, double dt) {
    return Bouncing(t + dt) - Bouncing(t);
}

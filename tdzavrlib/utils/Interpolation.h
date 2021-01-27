//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef INC_3DZAVR_INTERPOLATION_H
#define INC_3DZAVR_INTERPOLATION_H

#include "Point4D.h"
#include <cmath>

namespace Interpolation {
    static double Linear(double t);
    static double Cos(double t);
    static double Bezier(const Point4D& p1, const Point4D& p2, double t);

    static double dLinear(double t, double dt);
    static double dCos(double t, double dt);
    static double dBezier(const Point4D& p1, const Point4D& p2, double t, double dt);
};

double Interpolation::Linear(double t) {
    if(t >= 0)
        return ((int)trunc(t) % 2) ? 1.0 - (t-trunc(t)) : (t-trunc(t));
    else
        return Interpolation::Linear(-t);
}

double Interpolation::Cos(double t) {
    return 0.5*(1 - cos(M_PI*Interpolation::Linear(t)));
}

double Interpolation::Bezier(const Point4D &p1, const Point4D &p2, double t) {
    t = Interpolation::Linear(t);

    double h = 0.000001;
    double eps = 0.000001;

    // We are trying to find 's' when px = t
    auto f = [=](double s){
        return 3.0*(1.0-s)*(1.0-s)*s*p1.x + 3.0*(1.0-s)*s*s*p2.x + s*s*s - t;
    };
    // Using found 's' we will calculate resulting py
    auto py = [=](double s){
        return 3.0*(1.0-s)*(1.0-s)*s*p1.y + 3.0*(1.0-s)*s*s*p2.y + s*s*s;
    };

    auto df = [=](double s){
        return (f(s+h) - f(s-h))/(2.0*h);
    };

    // Newton method
    double s1 = 0.0, s2 = 0.5;
    int i = 0;

    while(abs(s1 - s2) > eps) {
        s1 = s2;
        s2 = s1 - f(s1) / df(s1);
        i++;
    }

    return py(s1);
}


double Interpolation::dLinear(double t, double dt) {
    return ((int)trunc(t) % 2) ? -dt : dt;
}

double Interpolation::dCos(double t, double dt) {
    return 0.5*M_PI*sin(M_PI*t)*dt;
}

double Interpolation::dBezier(const Point4D &p1, const Point4D &p2, double t, double dt) {
    return Interpolation::Bezier(p1, p2, t + dt) - Interpolation::Bezier(p1, p2, t);
}

#endif //INC_3DZAVR_INTERPOLATION_H

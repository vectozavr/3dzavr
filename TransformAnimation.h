//
// Created by Иван Ильин on 23.01.2021.
//

#ifndef INC_3DZAVR_TRANSFORMANIMATION_H
#define INC_3DZAVR_TRANSFORMANIMATION_H

#include "utils/Matrix4x4.h"
#include "utils/Time.h"

template <typename T>
class TransformAnimation {
public:
    enum Type{
        transition,
        rotation,
        wait,
        function
    };
private:
    double time = 0; // normalized time (from 0 to 1)

    double endAnimationPoint = 0;
    double startAnimationPoint = 0;
    Point4D point;
    Type t_type;

    double duration;

    bool started = false;

    Point4D bezier4[4] = {{0,0}, {0.8, 0}, {0.2, 1}, {1, 1}};
    double bezier(double dtime);
public:
    explicit TransformAnimation(Type t, const Point4D& p, double d);
    explicit TransformAnimation(Type t, double duration);

    bool update(T& obj);
    [[nodiscard]] Type type() const { return t_type; }
};

//
// Created by Иван Ильин on 23.01.2021.
//

template <typename T>
TransformAnimation<T>::TransformAnimation(Type t, const Point4D& p, double d) {
    duration = d;
    t_type = t;
    point = p;
}

template <typename T>
TransformAnimation<T>::TransformAnimation(TransformAnimation::Type t, double d) {
    duration = d;
    t_type = t;
    point = {0, 0, 0};
}

template <typename T>
double TransformAnimation<T>::bezier(double dtime) {

    double h = 0.000001;
    double eps = 0.000001;

    auto f = [=](double t, double px){
        return (1.0-t)*(1.0-t)*(1.0-t)*bezier4[0].x + 3.0*(1.0-t)*(1.0-t)*t*bezier4[1].x + 3.0*(1.0-t)*t*t*bezier4[2].x + t*t*t*bezier4[3].x - px;
    };

    auto py = [=](double t){
        return (1.0-t)*(1.0-t)*(1.0-t)*bezier4[0].y + 3.0*(1.0-t)*(1.0-t)*t*bezier4[1].y + 3.0*(1.0-t)*t*t*bezier4[2].y + t*t*t*bezier4[3].y;
    };

    auto df = [=](double t, double px){
        return (f(t+h, px) - f(t-h, px))/(2.0*h);
    };

    // Newton method
    double k1 = 0.0, k2 = 0.5;
    int s = 0;

    while(abs(k1 - k2) > eps) {
        k1 = k2;
        k2 = k1 - f(k1, time) / df(k1, time);
        s++;
    }

    double py1 = py(k1);

    k1 = 0.0, k2 = 0.5;
    int m = 0;

    while(abs(k1 - k2) > eps) {
        k1 = k2;
        k2 = k1 - f(k1, time+dtime) / df(k1, time+dtime);
        m++;
    }

    return py(k1) - py1;
}

template <typename T>
bool TransformAnimation<T>::update(T& obj) {
    if(!started) {
        startAnimationPoint = Time::time();
        endAnimationPoint = startAnimationPoint + duration;
    }
    started = true;

    double t_old = time;
    // linear normalized time:
    time = (Time::time() - startAnimationPoint)/(endAnimationPoint - startAnimationPoint);

    double dtime = time - t_old;
    // sin like progress:
    //double dp = 0.5*M_PI*sin(M_PI*time)*dt;
    // Bézier curves progress:


    //double dp = ( -p1 * 3*(1-time)*(1-time) - p2 * 6*time*(1-time) + p2 * 3*time*(1-time)*(1-time) - p3 * 3*time*time + p3 * 6*(1-time)*time + p4 * 3*time*time ).y*dt;
    //double dp = 3.0*((p2-p1)*(1-time)*(1-time) + (p3-p2)*2.0*time*(1.0-time) + (p4-p3)*time*time).y*dt;
    double dp = bezier(dtime);
    //double dp = dt;

    switch (t_type) {
        case transition:
            obj.translate(point * dp);
            break;
        case rotation:
            obj.rotate(point * dp);
            break;
        case wait:

            break;
        default: ;

    }

    return time < 0.999;
}

#endif //INC_3DZAVR_TRANSFORMANIMATION_H

//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef INC_3DZAVR_ANIMATION_H
#define INC_3DZAVR_ANIMATION_H

#include "utils/Time.h"
#include "utils/Interpolation.h"
#include "Triangle.h"

class Animation {
public:
    enum Type{
        translate,
        translateToPoint,
        attractToPoint,
        scale,
        rotate,
        rotateRelativePoint,
        rotateUpLeftLookAt,
        showCreation,
        wait,
    };
    enum InterpolationType {
        linear,
        cos,
        bezier
    };
private:
    double _time = 0; // normalized time (from 0 to 1)
    double _endAnimationPoint = 0;
    double _startAnimationPoint = 0;
    double _duration = 0;
    bool _started = false;
    // p - animation progress
    double _p = 0;
    double _dp = 0;
    // some useful parameters for dealing with animation
    Point4D _p_value{};
    Point4D _p_point{};
    double _d_value = 0;
    double _d_point = 0;
    Type _type;
    InterpolationType _intType = bezier;

    Point4D _bezier[2] = {{0.8, 0}, {0.2, 1}};

    std::vector<Triangle> _triangles;
public:
    explicit Animation(Type t, const Point4D& value, double duration, InterpolationType interpolationType = bezier);
    explicit Animation(Type t, const Point4D& point, const Point4D& value, double duration, InterpolationType interpolationType = bezier);
    explicit Animation(Type t, const Point4D& point, double value, double duration, InterpolationType interpolationType = bezier);
    explicit Animation(Type t, double duration, InterpolationType interpolationType = bezier);
    explicit Animation(Type t, std::vector<Triangle> tris, double duration, InterpolationType interpolationType = bezier);

    bool update();
    [[nodiscard]] double time() const { return _time; };
    [[nodiscard]] double p() const { return _p; };
    [[nodiscard]] double dp() const { return _dp; };
    [[nodiscard]] Point4D const& p_value() const { return _p_value; }
    [[nodiscard]] Point4D const& p_point() const { return _p_point; }
    [[nodiscard]] double d_value() const { return _d_value; }
    [[nodiscard]] double d_point() const { return _d_point; }
    [[nodiscard]] std::vector<Triangle>& triangles() { return _triangles; }

    void setValue(const Point4D& value) { _p_value = value; }
    void setPoint(const Point4D& point) { _p_point = point; }
    void setValue(double value) { _d_value = value; }
    void setPoint(double point) { _d_point = point; }
    void setType(Type type) { _type = type; }

    [[nodiscard]] Type type() const { return _type; }

    void setBezierParams(const Point4D& p1, const Point4D& p2) { _bezier[0] = p1; _bezier[1] = p2; }

    [[nodiscard]] bool started() const { return _started; }

};

#endif //INC_3DZAVR_ANIMATION_H

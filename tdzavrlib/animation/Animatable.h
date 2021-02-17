//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef INC_3DZAVR_ANIMATABLE_H
#define INC_3DZAVR_ANIMATABLE_H

#include <list>
#include "../Triangle.h"

//class Animation;
#include "Animation.h"

// All _objects in 3dzavr that should be animated must inherit class Animatable:
class Animatable {
protected:
    std::list<Animation*> animations;
public:
    Animatable() = default;
    // All methods about animation begins with 'a_'
    void a_translate(const Point4D& t, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_translateToPoint(const Point4D& point, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_attractToPoint(const Point4D& point, double value, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_rotate(const Point4D& r, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_rotateRelativePoint(const Point4D& point, const Point4D& r, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_rotateLeftUpLookAt(const Point4D& r, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_scale(const Point4D& s, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_scale(double s, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_rotateToAngle(const Point4D& r, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_rotateLeftUpLookAtToAngle(const Point4D& r, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);

    void a_decompose(double value, double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);

    void a_showCreation(double duration = 1, Animation::LoopOut looped = Animation::None, Animation::InterpolationType interpolationType = Animation::bezier);
    void a_wait(double duration = 1);

    void a_update();

    void a_stopAnimations() { animations.clear(); }

    [[nodiscard]] bool isInAnim() const { return !animations.empty(); }

    // methods to override:
    // If you want to create new animation you can either add new virtual function here
    // or override one of the following function:
    [[nodiscard]] virtual Point4D position() const { return Point4D{}; }
    [[nodiscard]] virtual Point4D angle() const { return Point4D{}; }
    [[nodiscard]] virtual Point4D angleLeftUpLookAt() const { return Point4D{}; }
    virtual void translate(const Point4D& dv) {}
    virtual void attractToPoint(const Point4D& point, double value) {}
    virtual void rotate(const Point4D& r) {}
    virtual void rotateLeftUpLookAt(const Point4D& r) {}
    virtual void rotateRelativePoint(const Point4D& point, const Point4D& r) {}
    [[nodiscard]] virtual std::vector<Triangle>& triangles() { return *(std::vector<Triangle>*)(new std::vector<Triangle>()); }
    virtual void setTriangles(const std::vector<Triangle>& tris) {}
    virtual void decompose(double value) {}
};

#endif //INC_3DZAVR_ANIMATABLE_H

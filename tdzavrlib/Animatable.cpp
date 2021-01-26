//
// Created by Иван Ильин on 26.01.2021.
//

#include "Animatable.h"
#include <iostream>

void Animatable::a_translate(const Point4D &t, double duration) {
    animations.emplace_back(Animation::translate, t, duration);
}

void Animatable::a_translateToPoint(const Point4D &point, double duration) {
    animations.emplace_back(Animation::translateToPoint, point, duration);
}

void Animatable::a_attractToPoint(const Point4D &point, double value, double duration) {
    animations.emplace_back(Animation::attractToPoint, point, value, duration);
}

void Animatable::a_rotate(const Point4D &r, double duration) {
    animations.emplace_back(Animation::rotate, r, duration);
}

void Animatable::a_rotateRelativePoint(const Point4D &point, const Point4D &r, double duration) {
    animations.emplace_back(Animation::rotateRelativePoint, point, r, duration);
}

void Animatable::a_rotateUpLeftLookAt(const Point4D &r, double duration) {
    animations.emplace_back(Animation::rotateUpLeftLookAt, r, duration);
}

void Animatable::a_scale(const Point4D &s, double duration) {
    animations.emplace_back(Animation::scale, s, duration);
}

void Animatable::a_scale(double s, double duration) {
    a_scale(Point4D{s, s, s}, duration);
}

void Animatable::a_showCreation(double duration) {
    animations.emplace_back(Animation::showCreation, triangles(), duration);
}

void Animatable::a_wait(double duration) {
    animations.emplace_back(Animation::wait, duration);
}

void Animatable::a_update() {
    if(animations.empty())
        return;
    auto it = animations.begin();

    // If it the first commend is wait we should wait until waiting time is over
    if(it->type() == Animation::wait)
    {
        if(!it->update())
            animations.erase(it);
        return;
    }


    // Otherwise we iterate over all animation until we meet animations.end() or wait animation
    while(!animations.empty() && (it != animations.end()) && (it->type() != Animation::wait))
    {
        if(!it->update())
            animations.erase(it++);
        else {
            Point4D dval = it->p_value() * it->dp();
            std::vector<Triangle> newTriangles;

            switch (it->type()) {
                case Animation::translate:
                    translate(dval);
                    break;
                case Animation::translateToPoint:
                    if(it->started()) {
                        it->setValue(it->p_value() - position());
                        it->setType(Animation::translate);
                    }
                    break;
                case Animation::attractToPoint:
                    attractToPoint(it->p_point(), it->dp() * it->d_value());
                    break;
                case Animation::rotate:
                    rotate(dval);
                    break;
                case Animation::rotateRelativePoint:
                    rotateRelativePoint(it->p_point(), dval);
                    break;
                case Animation::rotateUpLeftLookAt:
                    rotateUpLeftLookAt(dval);
                    break;
                case Animation::showCreation:
                    for(auto &t : it->triangles()) {
                        double d1 = (t[1] - t[0]).abs();
                        double d2 = (t[2] - t[1]).abs();
                        double sum = d1 + d2;
                        double a = it->p() * sum;
                        if(a < d1)
                            newTriangles.emplace_back(t[0], t[0], t[0] + (t[1] - t[0])*a/d1);
                        else
                            newTriangles.emplace_back(t[0], t[1], t[1] + (t[2] - t[1])*(a-d1)/d2);
                    }

                    setTriangles(newTriangles);
                    break;
                case Animation::wait:

                    break;
                default: ;
            }
            it++;
        }
    }
}


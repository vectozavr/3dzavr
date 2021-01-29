//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_AATTRACTTOPOINT_H
#define INC_3DZAVR_AATTRACTTOPOINT_H

#include "Animatable.h"
#include "Animation.h"

class AAttractToPoint : public Animation {
private:
    Point4D point;
    double value;
public:
    AAttractToPoint(const Point4D &p, double v, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        point = p;
        value = v;
    }

    bool update(Animatable& obj) {
        obj.attractToPoint(point, value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AATTRACTTOPOINT_H

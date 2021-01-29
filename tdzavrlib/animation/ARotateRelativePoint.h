//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_AROTATERELATIVEPOINT_H
#define INC_3DZAVR_AROTATERELATIVEPOINT_H

#include "Animatable.h"
#include "Animation.h"

class ARotateRelativePoint : public Animation {
private:
    Point4D point;
    Point4D value;
public:
    ARotateRelativePoint(const Point4D &p, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        point = p;
        value = r;
    }

    bool update(Animatable& obj) {
        obj.rotateRelativePoint(point, value * _dp);
        return updateState();
    }
};


#endif //INC_3DZAVR_AROTATERELATIVEPOINT_H

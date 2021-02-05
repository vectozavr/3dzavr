//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef INC_3DZAVR_AROTATETOANGLE_H
#define INC_3DZAVR_AROTATETOANGLE_H

#include "Animation.h"

class ARotateToAngle : public Animation {
private:
    Point4D value;
public:
    ARotateToAngle(const Point4D& r, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = r;
    }

    bool update(Animatable& obj) override {
        if(!_started)
            value = value - obj.angle();

        obj.rotate(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AROTATETOANGLE_H

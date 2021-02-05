//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef INC_3DZAVR_AROTATELEFTUPLOOKATTOANGLE_H
#define INC_3DZAVR_AROTATELEFTUPLOOKATTOANGLE_H

#include "Animation.h"
#include "../utils/Log.h"

class ARotateLeftUpLookAtToAngle : public Animation {
private:
    Point4D value;
public:
    ARotateLeftUpLookAtToAngle(const Point4D& r, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = r;
    }

    bool update(Animatable& obj) override {
        if(!_started)
            value = value - obj.angleLeftUpLookAt();

        obj.rotateLeftUpLookAt(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AROTATELEFTUPLOOKATTOANGLE_H

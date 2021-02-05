//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_AROTATELEFTUPLOOKAT_H
#define INC_3DZAVR_AROTATELEFTUPLOOKAT_H

#include "Animatable.h"
#include "Animation.h"

class ARotateLeftUpLookAt : public Animation {
private:
    Point4D value;
public:
    ARotateLeftUpLookAt(const Point4D& r, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = r;
    }

    bool update(Animatable& obj) override {
        obj.rotateLeftUpLookAt(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AROTATELEFTUPLOOKAT_H

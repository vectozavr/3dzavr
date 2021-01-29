//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_ADECOMPOSE_H
#define INC_3DZAVR_ADECOMPOSE_H

#include "Animatable.h"
#include "Animation.h"

class ADecompose : public Animation {
private:
    double value;
public:
    ADecompose(double v, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = v;
    }

    bool update(Animatable& obj) {
        obj.decompose(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_ADECOMPOSE_H

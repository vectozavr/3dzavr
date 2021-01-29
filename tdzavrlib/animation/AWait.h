//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_AWAIT_H
#define INC_3DZAVR_AWAIT_H

#include "Animatable.h"
#include "Animation.h"

class AWait : public Animation {
private:
    Point4D value;
public:
    explicit AWait(double duration) {
        _duration = duration;
        _intType = linear;
        _waitFor = true;
    }

    bool update(Animatable& obj) {
        return updateState();
    }
};

#endif //INC_3DZAVR_AWAIT_H

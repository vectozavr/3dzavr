//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_AROTATEUPLEFTLOOKAT_H
#define INC_3DZAVR_AROTATEUPLEFTLOOKAT_H

#include "Animatable.h"
#include "Animation.h"

class ARotateUpLeftLookAt : public Animation {
private:
    Point4D value;
public:
    ARotateUpLeftLookAt(const Point4D& r, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = r;
    }

    bool update(Animatable& obj) {
        obj.rotateUpLeftLookAt(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AROTATEUPLEFTLOOKAT_H

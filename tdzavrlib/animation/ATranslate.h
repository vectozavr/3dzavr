//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_ATRANSLATE_H
#define INC_3DZAVR_ATRANSLATE_H

#include "Animatable.h"
#include "Animation.h"

class ATranslate : public Animation {
private:
    Point4D value;
public:
    ATranslate(const Point4D& t, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = t;
    }

    bool update(Animatable& obj) {
        obj.translate(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_ATRANSLATE_H

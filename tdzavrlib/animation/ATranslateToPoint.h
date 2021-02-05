//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_ATRANSLATETOPOINT_H
#define INC_3DZAVR_ATRANSLATETOPOINT_H

#include "Animatable.h"
#include "Animation.h"

class ATranslateToPoint : public Animation {
private:
    Point4D point;
    Point4D value;
public:
    ATranslateToPoint(const Point4D& p, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        point = p;
    }

    bool update(Animatable& obj) override {
        if(!_started) {
            value = point - obj.position();
        }
        obj.translate(value * _dp);

        return updateState();
    }
};

#endif //INC_3DZAVR_ATRANSLATETOPOINT_H

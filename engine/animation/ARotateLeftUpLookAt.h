//
// Created by Иван Ильин on 01.11.2021.
//

#ifndef SHOOTER_AROTATELEFTUPLOOKAT_H
#define SHOOTER_AROTATELEFTUPLOOKAT_H

#include "Animation.h"
#include "../Object.h"


class ARotateLeftUpLookAt final : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotateLeft(_rotationValue.x()*dprogress());
        obj->rotateUp(_rotationValue.y()*dprogress());
        obj->rotateLookAt(_rotationValue.z()*dprogress());
    }

public:
    ARotateLeftUpLookAt(std::weak_ptr<Object> object, const Vec3D &r, double duration = 1, LoopOut looped = LoopOut::None,
            InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //SHOOTER_AROTATELEFT_H

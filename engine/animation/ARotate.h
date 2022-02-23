//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_AROTATE_H
#define ENGINE_AROTATE_H

#include "Animation.h"
#include "../Object.h"

class ARotate final : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotate(_rotationValue * dprogress());
    }

public:
    ARotate(std::weak_ptr<Object> object, const Vec3D &r, double duration = 1, LoopOut looped = LoopOut::None,
            InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {
    }
};

#endif //INC_3DZAVR_AROTATE_H

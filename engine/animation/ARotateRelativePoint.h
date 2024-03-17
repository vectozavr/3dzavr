#ifndef ANIMATION_AROTATERELATIVEPOINT_H
#define ANIMATION_AROTATERELATIVEPOINT_H

#include <animation/Animation.h>
#include "objects/Object.h"

class ARotateRelativePoint : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _targetPoint;
    const Vec3D _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotateRelativePoint(_targetPoint, _rotationValue * dprogress());
    }

public:
    ARotateRelativePoint(const std::weak_ptr<Object>& object, const Vec3D &targetPoint, const Vec3D &rotationValue,
                         double duration = 1, Animation::LoopOut looped = LoopOut::None,
                         Animation::InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _targetPoint(targetPoint),
              _rotationValue(rotationValue) {}
};

#endif //ANIMATION_AROTATERELATIVEPOINT_H

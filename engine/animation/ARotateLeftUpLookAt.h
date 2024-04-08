#ifndef ANIMATION_AROTATELEFTUPLOOKAT_H
#define ANIMATION_AROTATELEFTUPLOOKAT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"


class ARotateLeftUpLookAt final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
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
    ARotateLeftUpLookAt(const std::weak_ptr<TransformMatrix>& object, const Vec3D &r, double duration = 1, LoopOut looped = LoopOut::None,
            InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //ANIMATION_AROTATELEFTUPLOOKAT_H

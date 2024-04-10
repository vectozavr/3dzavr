#ifndef ANIMATION_ROTATELEFT_H
#define ANIMATION_ROTATELEFT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

class RotateLeft final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const double _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotateLeft(_rotationValue*dprogress());
    }

public:
    RotateLeft(const std::weak_ptr<TransformMatrix>& object, double r, double duration = 1, LoopOut looped = LoopOut::None,
               InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //ANIMATION_ROTATELEFT_H

#ifndef ANIMATION_AROTATELEFT_H
#define ANIMATION_AROTATELEFT_H

#include <animation/Animation.h>
#include "objects/Object.h"

class ARotateLeft final : public Animation {
private:
    const std::weak_ptr<Object> _object;
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
    ARotateLeft(const std::weak_ptr<Object>& object, double r, double duration = 1, LoopOut looped = LoopOut::None,
            InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //ANIMATION_AROTATELEFT_H

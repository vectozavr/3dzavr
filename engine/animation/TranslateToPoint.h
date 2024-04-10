#ifndef ANIMATION_TRANSLATETOPOINT_H
#define ANIMATION_TRANSLATETOPOINT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

class TranslateToPoint final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _targetPoint;
    Vec3D _translationValue;

    bool _started = false;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        if (!_started) {
            _started = true;
            _translationValue = _targetPoint - _object.lock()->position();
        }
        obj->translate(_translationValue * dprogress());
    }

public:
    TranslateToPoint(const std::weak_ptr<TransformMatrix>& object, const Vec3D &p, double duration = 1, LoopOut looped = LoopOut::None,
                     InterpolationType interpolationType = InterpolationType::Bezier)
                      : Animation(duration, looped, interpolationType), _targetPoint(p), _object(object) {
    }
};

#endif //ANIMATION_TRANSLATETOPOINT_H

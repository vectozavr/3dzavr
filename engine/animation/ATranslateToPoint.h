//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ATRANSLATETOPOINT_H
#define ENGINE_ATRANSLATETOPOINT_H

#include "Animation.h"
#include "../Object.h"

class ATranslateToPoint final : public Animation {
private:
    const std::weak_ptr<Object> _object;
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
    ATranslateToPoint(std::weak_ptr<Object> object, const Vec3D &p, double duration = 1, LoopOut looped = LoopOut::None,
                      InterpolationType interpolationType = InterpolationType::Bezier)
                      : Animation(duration, looped, interpolationType), _targetPoint(p), _object(object) {
    }
};

#endif //INC_3DZAVR_ATRANSLATETOPOINT_H

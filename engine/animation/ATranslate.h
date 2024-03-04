#ifndef ANIMATION_ATRANSLATE_H
#define ANIMATION_ATRANSLATE_H

#include <animation/Animation.h>
#include "objects/Object.h"

class ATranslate final : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _translationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->translate(_translationValue * dprogress());
    }

public:
    ATranslate(std::weak_ptr<Object> object, const Vec3D &t, double duration = 1, LoopOut looped = LoopOut::None,
               InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                            interpolationType),
                                                                                  _object(object),
                                                                                  _translationValue(t) {
    }
};

#endif //ANIMATION_ATRANSLATE_H

#ifndef ANIMATION_TRANSLATE_H
#define ANIMATION_TRANSLATE_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

class Translate final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
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
    Translate(const std::weak_ptr<TransformMatrix>& object, const Vec3D &t, double duration = 1, LoopOut looped = LoopOut::None,
              InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                            interpolationType),
                                                                                  _object(object),
                                                                                  _translationValue(t) {
    }
};

#endif //ANIMATION_TRANSLATE_H

#ifndef ANIMATION_ASCALE_H
#define ANIMATION_ASCALE_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

class AScale final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _scalingValue;
    Vec3D _prevScaleFactor{1, 1, 1};

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }
        // invert scale
        obj->scale(Vec3D(1.0/_prevScaleFactor.x(), 1.0/_prevScaleFactor.y(), 1.0/_prevScaleFactor.z()));
        Vec3D scaleFactor = Vec3D{1, 1, 1} + (_scalingValue - Vec3D{1, 1, 1}) * progress();
        obj->scale(scaleFactor);
        _prevScaleFactor = scaleFactor;
    }

public:
    AScale(const std::weak_ptr<TransformMatrix>& object, const Vec3D &s, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _object(object), _scalingValue(s) {
    }
};

#endif //ANIMATION_ASCALE_H

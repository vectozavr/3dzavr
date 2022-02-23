//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ASCALE_H
#define ENGINE_ASCALE_H

#include "Animation.h"
#include "../Object.h"

class AScale final : public Animation {
private:
    const std::weak_ptr<Object> _object;
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
    AScale(std::weak_ptr<Object> object, const Vec3D &s, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _object(object), _scalingValue(s) {
    }
};

#endif //INC_3DZAVR_ASCALE_H

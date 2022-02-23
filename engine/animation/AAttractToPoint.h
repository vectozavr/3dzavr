//
// Created by Иван Ильин on 01.11.2021.
//

#ifndef SHOOTER_AATTRACTTOPOINT_H
#define SHOOTER_AATTRACTTOPOINT_H

#include "Animation.h"
#include "../Object.h"

class AAttractToPoint : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _targetPoint;
    const double _valueToAttract;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->attractToPoint(_targetPoint, _valueToAttract * dprogress());
    }

public:
    AAttractToPoint(std::weak_ptr<Object> object, const Vec3D &targetPoint, double valueToAttract, double duration = 1,
                    Animation::LoopOut looped = LoopOut::None,
                    Animation::InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _targetPoint(targetPoint),
              _valueToAttract(valueToAttract) {}
};

#endif //SHOOTER_AATTRACTTOPOINT_H

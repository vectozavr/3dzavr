#ifndef ANIMATION_ATTRACTTOPOINT_H
#define ANIMATION_ATTRACTTOPOINT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

class AttractToPoint : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
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
    AttractToPoint(const std::weak_ptr<TransformMatrix>& object, const Vec3D &targetPoint, double valueToAttract, double duration = 1,
                   Animation::LoopOut looped = LoopOut::None,
                   Animation::InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _targetPoint(targetPoint),
              _valueToAttract(valueToAttract) {}
};

#endif //ANIMATION_ATTRACTTOPOINT_H

#ifndef ANIMATION_ATTRACTTOPOINT_H
#define ANIMATION_ATTRACTTOPOINT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class AttractToPoint
 * @brief Animates the attraction of an object towards a specified point over time.
 *
 * This class extends the Animation base class to move an object towards a target point. The movement
 * is based on a specified attraction value that controls the speed of the attraction. The update method
 * calculates the new position of the object at each frame, progressively moving it closer to the target
 * point as the animation progresses.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _targetPoint The target point towards which the object is attracted.
 * _valueToAttract The value controlling the speed of the attraction towards the target point.
 *
 * @public_section
 * AttractToPoint Constructor that initializes the animation with the target object, target point, attraction value,
 *                 duration, looping behavior, and interpolation type.
 *                 @param object A weak pointer to the TransformMatrix of the object to animate.
 *                 @param targetPoint The target point towards which the object will be attracted.
 *                 @param valueToAttract The value controlling the speed of the attraction.
 *                 @param duration The duration of the animation in seconds.
 *                 @param looped The looping behavior of the animation (None or Continue).
 *                 @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the attraction towards the target point based on the current progress of the animation.
 */
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

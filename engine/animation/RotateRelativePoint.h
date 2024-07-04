#ifndef ANIMATION_ROTATERELATIVEPOINT_H
#define ANIMATION_ROTATERELATIVEPOINT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class RotateRelativePoint
 * @brief Animates the rotation of an object around a specified point.
 *
 * This class is a specialized animation derived from the Animation base class, designed to rotate an object
 * around a given point by a specified rotation vector over time. It leverages the update mechanism of the
 * Animation class to progressively apply the rotation, adjusting the object's orientation each frame until
 * the animation duration is completed.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _targetPoint The point around which the object is rotated. This point remains fixed throughout the animation.
 * _rotationValue The rotation vector specifying the direction and magnitude of the rotation.
 *                This vector is applied progressively to the object's orientation based on the animation's progress.
 *
 * @public_section
 * RotateRelativePoint Constructor that initializes the animation with the target object, target point for rotation,
 *                     rotation vector, duration, looping behavior, and interpolation type.
 *                     @param object A weak pointer to the TransformMatrix of the object to animate.
 *                     @param targetPoint The point around which the object will be rotated.
 *                     @param rotationValue The rotation vector specifying the rotation.
 *                     @param duration The duration of the animation in seconds.
 *                     @param looped The looping behavior of the animation (None or Continue).
 *                     @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the rotation around the specified point based on the current progress of the animation.
 */
class RotateRelativePoint : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _targetPoint;
    const Vec3D _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotateRelativePoint(_targetPoint, _rotationValue * dprogress());
    }

public:
    RotateRelativePoint(const std::weak_ptr<TransformMatrix>& object, const Vec3D &targetPoint, const Vec3D &rotationValue,
                        double duration = 1, Animation::LoopOut looped = LoopOut::None,
                        Animation::InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _targetPoint(targetPoint),
              _rotationValue(rotationValue) {}
};

#endif //ANIMATION_ROTATERELATIVEPOINT_H

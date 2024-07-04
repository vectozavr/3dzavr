#ifndef ANIMATION_ROTATELEFTUPLOOKAT_H
#define ANIMATION_ROTATELEFTUPLOOKAT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class RotateLeftUpLookAt
 * @brief Animates an object's rotation around its left, up, and look-at axes.
 *
 * This class is a specialized animation derived from the Animation base class, designed to rotate an object
 * around its left, up, and look-at axes by specified amounts over time. It leverages the update mechanism of
 * the Animation class to progressively apply the rotation, adjusting the object's orientation each frame until
 * the animation duration is completed. This allows for complex rotational animations that can simulate looking
 * around or orienting towards different directions.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _rotationValue The rotation vector specifying the direction and magnitude of the rotation around the left,
 *                up, and look-at axes. These values are applied progressively to the object's orientation based
 *                on the animation's progress.
 *
 * @public_section
 * RotateLeftUpLookAt Constructor that initializes the animation with the target object, rotation vector, duration,
 *                    looping behavior, and interpolation type.
 *                    @param object A weak pointer to the TransformMatrix of the object to animate.
 *                    @param r The rotation vector specifying the rotation around the left, up, and look-at axes.
 *                    @param duration The duration of the animation in seconds.
 *                    @param looped The looping behavior of the animation (None or Continue).
 *                    @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the rotation around the left, up, and look-at axes based on the current progress of the animation.
 */
class RotateLeftUpLookAt final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotateLeft(_rotationValue.x()*dprogress());
        obj->rotateUp(_rotationValue.y()*dprogress());
        obj->rotateLookAt(_rotationValue.z()*dprogress());
    }

public:
    RotateLeftUpLookAt(const std::weak_ptr<TransformMatrix>& object, const Vec3D &r, double duration = 1, LoopOut looped = LoopOut::None,
                       InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //ANIMATION_ROTATELEFTUPLOOKAT_H

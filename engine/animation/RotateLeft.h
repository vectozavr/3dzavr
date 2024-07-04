#ifndef ANIMATION_ROTATELEFT_H
#define ANIMATION_ROTATELEFT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class RotateLeft
 * @brief Animates the rotation of an object over its left axis time.
 *
 * This class is a specialized animation derived from the Animation base class, designed to rotate an object
 * relative to left axis by a specified angle over time. It leverages the update mechanism of the Animation class to
 * progressively apply the rotation, adjusting the object's orientation each frame until the animation duration
 * is completed.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _rotationValue The angle by which the object is rotated left. This value is applied progressively to the object's
 *                orientation based on the animation's progress.
 *
 * @public_section
 * RotateLeft Constructor that initializes the animation with the target object, rotation angle, duration,
 *             looping behavior, and interpolation type.
 *             @param object A weak pointer to the TransformMatrix of the object to animate.
 *             @param r The amount of rotation.
 *             @param duration The duration of the animation in seconds.
 *             @param looped The looping behavior of the animation (None or Continue).
 *             @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the rotation to the object based on the current progress of the animation.
 */
class RotateLeft final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const double _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotateLeft(_rotationValue*dprogress());
    }

public:
    RotateLeft(const std::weak_ptr<TransformMatrix>& object, double r, double duration = 1, LoopOut looped = LoopOut::None,
               InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //ANIMATION_ROTATELEFT_H

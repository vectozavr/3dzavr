#ifndef ANIMATION_ROTATE_H
#define ANIMATION_ROTATE_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class Rotate
 * @brief Animates the rotation of an object around XYZ axes.
 *
 * This class is a specialized animation derived from the Animation base class, designed to rotate an object
 * around standard XYZ axes over time. It leverages the update mechanism of the
 * Animation class to progressively apply the rotation, adjusting the object's orientation each frame until
 * the animation duration is completed.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _rotationValue The rotation vector specifying the magnitude of the rotation for X, Y and Z axes.
 *                Rotation is applied progressively to the object's orientation based on the animation's progress.
 *
 * @public_section
 * Rotate Constructor that initializes the animation with the target object, rotation vector, duration,
 *       looping behavior, and interpolation type.
 *       @param object A weak pointer to the TransformMatrix of the object to animate.
 *       @param r XYZ rotation.
 *       @param duration The duration of the animation in seconds.
 *       @param looped The looping behavior of the animation (None or Continue).
 *       @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the rotation based on the current progress of the animation.
 */
class Rotate final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _rotationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->rotate(_rotationValue * dprogress());
    }

public:
    Rotate(const std::weak_ptr<TransformMatrix>& object, const Vec3D &r, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {
    }
};

#endif //ANIMATION_ROTATE_H

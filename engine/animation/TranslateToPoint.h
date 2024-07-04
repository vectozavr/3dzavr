#ifndef ANIMATION_TRANSLATETOPOINT_H
#define ANIMATION_TRANSLATETOPOINT_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class TranslateToPoint
 * @brief Animates the translation of an object to a specified point.
 *
 * This class is a specialized animation that moves an object to a target point over time. It extends the
 * Animation base class, utilizing its update mechanism to progressively translate the object each frame
 * until it reaches the target location. The translation is calculated based on the difference between the
 * object's current position and the target point, adjusted by the animation's progress.
 *
 * @inherit Animation The base class providing the framework for animation timing and progression.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _targetPoint The 3D point to which the object is being animated.
 * _translationValue The calculated translation vector needed to move the object from its initial position to
 *                   the target point. This value is recalculated when the animation starts.
 * _started A flag indicating whether the animation has started. This is used to perform initial setup steps
 *          the first time the update method is called.
 *
 * @public_section
 * TranslateToPoint Constructor that initializes the animation with the target object, target point, duration,
 *                   looping behavior, and interpolation type.
 *                   @param object A weak pointer to the TransformMatrix of the object to animate.
 *                   @param p The target point to animate the object to.
 *                   @param duration The duration of the animation in seconds.
 *                   @param looped The looping behavior of the animation (None or Continue).
 *                   @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the translation needed to move the object towards the target point based on the current progress
 *            of the animation.
 */
class TranslateToPoint final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _targetPoint;
    Vec3D _translationValue;

    bool _started = false;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        if (!_started) {
            _started = true;
            _translationValue = _targetPoint - _object.lock()->position();
        }
        obj->translate(_translationValue * dprogress());
    }

public:
    TranslateToPoint(const std::weak_ptr<TransformMatrix>& object, const Vec3D &p, double duration = 1, LoopOut looped = LoopOut::None,
                     InterpolationType interpolationType = InterpolationType::Bezier)
                      : Animation(duration, looped, interpolationType), _targetPoint(p), _object(object) {
    }
};

#endif //ANIMATION_TRANSLATETOPOINT_H

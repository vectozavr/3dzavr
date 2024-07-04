#ifndef ANIMATION_TRANSLATE_H
#define ANIMATION_TRANSLATE_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class Translate
 * @brief Animates the translation of an object to a specified position.
 *
 * This class is a specialized animation derived from the Animation base class, designed to move an object
 * by a specified translation vector over time. It leverages the update mechanism of the Animation class
 * to progressively apply the translation, adjusting the object's position each frame until the animation
 * duration is completed.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _translationValue The translation vector specifying the direction and magnitude of the object's movement.
 *                   This vector is applied progressively to the object's position based on the animation's progress.
 *
 * @public_section
 * Translate Constructor that initializes the animation with the target object, translation vector, duration,
 *              looping behavior, and interpolation type.
 *              @param object A weak pointer to the TransformMatrix of the object to animate.
 *              @param t The translation vector specifying the movement.
 *              @param duration The duration of the animation in seconds.
 *              @param looped The looping behavior of the animation (None or Continue).
 *              @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the translation to the object based on the current progress of the animation.
 */
class Translate final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
    const Vec3D _translationValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        obj->translate(_translationValue * dprogress());
    }

public:
    Translate(const std::weak_ptr<TransformMatrix>& object, const Vec3D &t, double duration = 1, LoopOut looped = LoopOut::None,
              InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                            interpolationType),
                                                                                  _object(object),
                                                                                  _translationValue(t) {
    }
};

#endif //ANIMATION_TRANSLATE_H

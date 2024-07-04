#ifndef ANIMATION_SCALE_H
#define ANIMATION_SCALE_H

#include <animation/Animation.h>
#include "components/TransformMatrix.h"

/**
 * @class Scale
 * @brief Animates the scaling of an object over time.
 *
 * This class is a specialized animation derived from the Animation base class, designed to scale an object
 * by a specified scaling vector over time. It leverages the update mechanism of the Animation class to
 * progressively apply the scaling, adjusting the object's scale each frame until the animation duration
 * is completed.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _object A weak pointer to the TransformMatrix of the object being animated. This ensures that the animation
 *         does not prevent the object from being destructed and handles the case where the object might be
 *         deleted before the animation completes.
 * _scalingValue The scaling vector specifying the direction and magnitude of the object's scaling.
 *               This vector is applied progressively to the object's scale based on the animation's progress.
 * _prevScaleFactor Stores the previous scale factor to correctly apply incremental scaling.
 *
 * @public_section
 * Scale Constructor that initializes the animation with the target object, scaling vector, duration,
 *       looping behavior, and interpolation type.
 *       @param object A weak pointer to the TransformMatrix of the object to animate.
 *       @param s The scaling vector specifying the scaling transformation.
 *       @param duration The duration of the animation in seconds.
 *       @param looped The looping behavior of the animation (None or Continue).
 *       @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies
 *            the scaling to the object based on the current progress of the animation. It first inverts the
 *            previous scaling to maintain a correct scale transformation and then applies the new scaling.
 */
class Scale final : public Animation {
private:
    const std::weak_ptr<TransformMatrix> _object;
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
    Scale(const std::weak_ptr<TransformMatrix>& object, const Vec3D &s, double duration = 1, LoopOut looped = LoopOut::None,
          InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _object(object), _scalingValue(s) {
    }
};

#endif //ANIMATION_SCALE_H

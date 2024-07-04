#ifndef ANIMATION_WAIT_H
#define ANIMATION_WAIT_H

#include <animation/Animation.h>

/**
 * @class Wait
 * @brief A specialized animation that represents a waiting period.
 *
 * This class is derived from the Animation base class and represents a waiting period within an animation sequence.
 * It essentially pauses the animation timeline for a specified duration. This can be useful for timing purposes,
 * creating delays between animations, or simply pausing the animation flow. The Wait animation does not perform
 * any visual changes during its update cycle; it merely consumes time.
 *
 * @inherit Animation The base class for all animations, providing common functionality such as duration management,
 * looping behavior, and interpolation type, although the Wait class does not utilize interpolation.
 *
 * @fn update() The override of the pure virtual update function from the Animation class. For the Wait class,
 * this function is a no-op, as the Wait animation does not change any state during its update cycle.
 *
 * @constructor Wait(double duration = 1) Constructs a Wait animation with a specified duration. By default,
 * the duration is set to 1 second. The constructor initializes the Wait animation with no looping, linear
 * interpolation (unused), and sets it to wait for the completion of other animations before starting.
 *
 * @param duration The duration of the wait period in seconds. Defaults to 1 second if not specified.
 */
class Wait final : public Animation {
private:
    void update() override {}

public:
    explicit Wait(double duration = 1) : Animation(duration, LoopOut::None, InterpolationType::Linear, true) {
    }
};

#endif //ANIMATION_WAIT_H

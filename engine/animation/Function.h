#ifndef ANIMATION_FUNCTION_H
#define ANIMATION_FUNCTION_H

#include <functional>

#include <animation/Animation.h>

/**
 * @class Function
 * @brief Executes a callback function a specified number of times during the animation.
 *
 * This class extends the Animation class to execute a given callback function a specific number of times
 * evenly distributed over the animation's duration. It is useful for triggering actions at certain points
 * during an animation, such as sound effects or visual changes.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _callsCounter The number of times the callback has been called so far.
 * _allCalls The total number of times the callback should be called during the animation.
 * _callBack The callback function to be executed.
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It checks the animation's
 *            progress and executes the callback function at intervals based on the total number of calls.
 *
 * @public_section
 * Function Constructor that initializes the animation with a callback function, number of calls, duration,
 *             looping behavior, and interpolation type.
 *             @param function The callback function to execute.
 *             @param calls The number of times the callback should be executed during the animation.
 *             @param duration The duration of the animation in seconds.
 *             @param looped The looping behavior of the animation (None or Continue).
 *             @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 */
class Function final : public Animation {
private:
    int _callsCounter = 0;
    const int _allCalls = 1;
    const std::function<void()> _callBack;

    void update() override {
        if (_allCalls != 0 && progress() >= (double) (_callsCounter + 1) / _allCalls) {
            _callsCounter++;
            _callBack();
        }
    }

public:
    explicit Function(std::function<void()> function, int calls = 1, double duration = 1,
                      LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::Linear)
            : Animation(duration, looped, interpolationType), _callBack(std::move(function)), _allCalls(calls) {
    }
};

#endif //ANIMATION_FUNCTION_H

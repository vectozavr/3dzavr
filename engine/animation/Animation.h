#ifndef ANIMATION_ANIMATION_H
#define ANIMATION_ANIMATION_H

#include <animation/Interpolation.h>

/**
 * @class Animation
 * @brief Base class for creating animations with various interpolation types and loop behaviors.
 *
 * This class provides a framework for defining animations with customizable duration, looping behavior,
 * and interpolation type. It supports linear, bezier, and bouncing interpolations, and can either loop
 * continuously or stop after one iteration. Animations can also be set to wait for the completion of
 * other animations before starting.
 *
 * @enum InterpolationType Defines the type of interpolation used for the animation.
 * @enum LoopOut Defines the looping behavior of the animation.
 *
 * @var _time Normalized time of the animation, ranging from 0 to 1.
 * @var _dtime Delta time used to update the animation's progress.
 * @var _finished Flag indicating whether the animation has finished.
 * @var _progress Current progress of the animation.
 * @var _dprogress Delta progress used to update the animation's state.
 * @var _waitForFinish If true, the animation will wait for all animations to finish before starting.
 * @var _duration Duration of the animation in seconds.
 * @var _looped Looping behavior of the animation.
 * @var _intType Interpolation type used for the animation.
 *
 * @function update Pure virtual function that should be overridden to define the animation update logic.
 * @function progress Getter for the animation's current progress.
 * @function dprogress Getter for the animation's delta progress.
 * @function stop Stops the animation.
 * @function Animation Constructor initializing the animation with duration, loop behavior, interpolation type, and wait-for-finish flag.
 * @function ~Animation Default virtual destructor.
 * @function updateState Updates the state of the animation based on the delta time. Returns true if the animation is finished.
 * @function isWaitingForFinish Checks if the animation is set to wait for other animations to finish before starting.
 */
class Animation {
public:
    enum class InterpolationType {
        Linear,
        Bezier,
        Bouncing
    };
    enum class LoopOut {
        None,
        Continue
    };
private:
    // normalized time (from 0 to 1)
    double _time = 0;
    double _dtime = 0;

    bool _finished = false;

    double _progress = 0;
    double _dprogress = 0;

    // If '_waitForFinish' == true then we need to finish all animation before starting this one. (for example AWait)
    // In addition new animations in particular animation list will be started only after finishing this animation.
    const bool _waitForFinish;
    const double _duration = 0;
    const LoopOut _looped = LoopOut::None;
    const InterpolationType _intType = InterpolationType::Bezier;

    // You should override this method for your particular animation
    virtual void update() = 0;

protected:
    [[nodiscard]] double progress() const { return _progress; }
    [[nodiscard]] double dprogress() const { return _dprogress; }

    void stop() { _finished = true; }

public:
    Animation(double duration, LoopOut looped, InterpolationType intType, bool waitForFinish = false);

    virtual ~Animation() = default;

    bool updateState();

    [[nodiscard]] bool isWaitingForFinish() const { return _waitForFinish; }
};

#endif //ANIMATION_ANIMATION_H

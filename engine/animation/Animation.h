//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef ENGINE_ANIMATION_H
#define ENGINE_ANIMATION_H

#include "Interpolation.h"

class Animation {
public:
    enum class InterpolationType {
        Linear,
        Cos,
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

#endif //INC_3DZAVR_ANIMATION_H

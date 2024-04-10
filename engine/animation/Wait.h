#ifndef ANIMATION_WAIT_H
#define ANIMATION_WAIT_H

#include <animation/Animation.h>

class Wait final : public Animation {
private:
    void update() override {}

public:
    explicit Wait(double duration = 1) : Animation(duration, LoopOut::None, InterpolationType::Linear, true) {
    }
};

#endif //ANIMATION_WAIT_H

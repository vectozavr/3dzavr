#ifndef ANIMATION_AWAIT_H
#define ANIMATION_AWAIT_H

#include <animation/Animation.h>

class AWait final : public Animation {
private:
    void update() override {}

public:
    explicit AWait(double duration = 1) : Animation(duration, LoopOut::None, InterpolationType::Linear, true) {
    }
};

#endif //ANIMATION_AWAIT_H

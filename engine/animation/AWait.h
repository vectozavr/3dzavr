//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_AWAIT_H
#define ENGINE_AWAIT_H

#include "Animation.h"

class AWait final : public Animation {
private:
    void update() override {}

public:
    explicit AWait(double duration = 1) : Animation(duration, LoopOut::None, InterpolationType::Linear, true) {
    }
};

#endif //INC_3DZAVR_AWAIT_H

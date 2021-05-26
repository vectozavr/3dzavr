//
// Created by Иван Ильин on 06.04.2021.
//

#include <utility>

#include "Animation.h"

#ifndef MINECRAFT_3DZAVR_AFUNCTION_H
#define MINECRAFT_3DZAVR_AFUNCTION_H

class AFunction : public Animation {
private:
    int _calls = 0;
    int _allCalls = 3;
    std::function<void()> _callBack;

public:
    AFunction(std::function<void()> function, int calls, double duration, LoopOut looped, InterpolationType interpolationType) {
        _callBack = std::move(function);
        _allCalls = calls;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
    }

    bool update(Animatable& obj) override {
        if(_allCalls != 0 && _p > (double)_calls / _allCalls) {
            _calls++;
            _callBack();
        }
        return updateState();
    }
};

#endif //MINECRAFT_3DZAVR_AFUNCTION_H

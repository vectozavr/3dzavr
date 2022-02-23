//
// Created by Иван Ильин on 27.01.2021.
//

#include "Animation.h"
#include "../Consts.h"
#include "../utils/Time.h"

Animation::Animation(double duration, Animation::LoopOut looped, Animation::InterpolationType intType, bool waitForFinish)
        : _duration(duration), _looped(looped), _intType(intType), _waitForFinish(waitForFinish) {
}

bool Animation::updateState() {
    if (_finished || std::abs(_duration) < Consts::EPS) {
        _finished = true;
        return false;
    }

    // linear normalized time:
    _dtime = Time::deltaTime() / _duration;

    switch (_intType) {
        case InterpolationType::Bezier:
            _dprogress = Interpolation::dBezier(Consts::BEZIER[0], Consts::BEZIER[1], _time, _dtime);
            break;
        case InterpolationType::Bouncing:
            _dprogress = Interpolation::dBouncing(_time, _dtime);
            break;
        case InterpolationType::Linear:
            _dprogress = Interpolation::dLinear(_time, _dtime);
            break;
        case InterpolationType::Cos:
            _dprogress = Interpolation::dCos(_time, _dtime);
            break;
        default:
            throw std::logic_error{
                    "Animation::updateState: unknown interpolation type " + std::to_string(static_cast<int>(_intType))
            };
    }

    if (_time + _dtime > 1.0) {
        _dtime = 1.0 - _time;
        _time = 1.0;
        _dprogress = 1.0 - _progress;
        _progress = 1.0;
        _finished = true;

    } else {
        _time += _dtime;
        _progress += _dprogress;
    }

    if (_looped == LoopOut::Continue && _time > 0.5) {
        _time = 0.5;
    }

    update();

    return !_finished;
}

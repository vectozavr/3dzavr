//
// Created by Иван Ильин on 27.01.2021.
//

#include "Animation.h"

#include <utility>
#include "../utils/Log.h"

bool Animation::updateState() {
    if(!_started) {
        _startAnimationPoint = Time::time();
        _endAnimationPoint = _startAnimationPoint + _duration;
        _started = true;
        return _duration != 0;
    }

    _timeOld = _time;
    // linear normalized time:
    _time = (Time::time() - _startAnimationPoint)/(_endAnimationPoint - _startAnimationPoint);

    if(_looped != Continue || _time < 0.5)
        _dtime = _time - _timeOld;
    else {
        _time = _timeOld;
        //_intType = linear;
    }

    switch (_intType) {
        case bezier:
            _p = Interpolation::Bezier(_bezier[0], _bezier[1], _time);
            _dp = Interpolation::dBezier(_bezier[0], _bezier[1], _time, _dtime);
            break;
        case bouncing:
            _p = Interpolation::Bouncing(_time);
            _dp = Interpolation::dBouncing(_time, _dtime);
            break;
        case linear:
            _p = Interpolation::Linear(_time);
            _dp = Interpolation::dLinear(_time, _dtime);
            break;
        case cos:
            _p = Interpolation::Cos(_time);
            _dp = Interpolation::dCos(_time, _dtime);
            break;
    }

    return (_time < 1) || _looped == Cycle;
}
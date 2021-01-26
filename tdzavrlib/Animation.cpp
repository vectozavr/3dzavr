//
// Created by Иван Ильин on 27.01.2021.
//

#include "Animation.h"
#include "utils/Log.h"

Animation::Animation(Type t, const Point4D& value, double duration, InterpolationType interpolationType) : _type(t), _p_value(value), _duration(duration), _intType(interpolationType){

}
Animation::Animation(Type t, const Point4D& point, const Point4D& value, double duration, InterpolationType interpolationType) : _type(t), _p_point(point), _p_value(value), _duration(duration), _intType(interpolationType){

}
Animation::Animation(Type t, const Point4D& point, double value, double duration, InterpolationType interpolationType) : _type(t), _p_point(point), _d_value(value), _duration(duration), _intType(interpolationType){

}
Animation::Animation(Type t, double duration, InterpolationType interpolationType) : _type(t), _duration(duration), _intType(interpolationType) {

}
Animation::Animation(Type t, std::vector<Triangle> tris, double duration, InterpolationType interpolationType) : _type(t), _duration(duration), _intType(interpolationType) {
    _triangles = tris;
}

bool Animation::update() {
    if(!_started) {
        _startAnimationPoint = Time::time();
        _endAnimationPoint = _startAnimationPoint + _duration;
    }

    double t_old = _time;
    // linear normalized time:
    _time = (Time::time() - _startAnimationPoint)/(_endAnimationPoint - _startAnimationPoint);

    double dtime = _time - t_old;

    switch (_intType) {
        case bezier:
            _p = Interpolation::Bezier(_bezier[0], _bezier[1], _time);
            _dp = Interpolation::dBezier(_bezier[0], _bezier[1], _time, dtime);
            break;
        case linear:
            _p = Interpolation::Linear(_time);
            _dp = Interpolation::dLinear(dtime);
            break;
        case cos:
            _p = Interpolation::Cos(_time);
            _dp = Interpolation::dCos(_time, dtime);
            break;
    }

    _started = true;
    return _time < 0.999;
}
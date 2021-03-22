//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef INC_3DZAVR_ANIMATION_H
#define INC_3DZAVR_ANIMATION_H

#include "../utils/Time.h"
#include "../Triangle.h"
#include "Interpolation.h"

class Animatable;

class Animation {
public:
    enum InterpolationType {
        linear,
        cos,
        bezier,
        bouncing
    };
    enum LoopOut {
        None,
        Cycle,
        Continue
    };
protected:
    double _time = 0; // normalized time (from 0 to 1)
    double _dtime = 0;
    double _timeOld = 0;

    double _endAnimationPoint = 0;
    double _startAnimationPoint = 0;
    double _duration = 0;
    bool _started = false;
    LoopOut _looped = None;
    // p - animation progress
    double _p = 0;
    double _dp = 0;

    InterpolationType _intType = bezier;
    Point4D _bezier[2] = {{0.8, 0}, {0.2, 1}};

    // If '_waitFor' == true then we need to finish all animation before starting this one. (for example for a_wait() or a_scale())
    bool _waitFor = false;

    bool updateState();
public:
    Animation() = default;
    virtual ~Animation() = default;

    void setBezierParams(const Point4D& p1, const Point4D& p2) { _bezier[0] = p1; _bezier[1] = p2; }
    [[nodiscard]] bool waitFor() const { return _waitFor; }


    // You should override this method for your particular animation
    virtual bool update(Animatable& obj) = 0;

    [[nodiscard]] virtual int type() const{return 0;}
};

#endif //INC_3DZAVR_ANIMATION_H

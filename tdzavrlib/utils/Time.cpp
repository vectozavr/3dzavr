//
// Created by Иван Ильин on 11.01.2021.
//

#include "Time.h"
#include <chrono>
//#define FIXED_STEP (1.0/60.0)
#define BIG_STEP (1.0/15.0)

using namespace std::chrono;

namespace Time
{
    namespace
    {
        // High precision time
        high_resolution_clock::time_point _start = high_resolution_clock::now();
        high_resolution_clock::time_point _last;

        // FPS counter
        high_resolution_clock::time_point _fpsStart;
        milliseconds _fpsCountTime = milliseconds(1000);
        int _fpsCounter = 0;
        double _lastFps = 0;

        // Compatibility
        double _time;
        double _deltaTime;
        double _realDeltaTime;
    }

    double time()
    {
        return _time;
    }

    double deltaTime()
    {
        return _deltaTime;
    }

    double realDeltaTime()
    {
        return _realDeltaTime;
    }

    void update()
    {
        high_resolution_clock::time_point t = high_resolution_clock::now();

#ifdef FIXED_STEP
        _deltaTime = FIXED_STEP;
        _time += FIXED_STEP;
#else
        _deltaTime = duration<double>(t - _last).count();
        _time = duration<double>(t - _start).count();
        // in case when fps < 10 it is useful to decrease _deltaTime (to avoid collision problems)
        if(_deltaTime > BIG_STEP)
            _deltaTime = BIG_STEP;
#endif
        _realDeltaTime = duration<double>(t - _last).count();
        _last = t;

        if(_deltaTime > 10000)
            return;

        _fpsCounter++;
        if (t - _fpsStart > _fpsCountTime)
        {
            _lastFps = _fpsCounter / duration<double>(t - _fpsStart).count();
            _fpsCounter = 0;
            _fpsStart = t;
        }
    }

    int fps()
    {
        // Cast is faster than floor and has the same behavior for positive numbers 
        return static_cast<int>(_lastFps);
    }
}

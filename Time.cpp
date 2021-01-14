//
// Created by Иван Ильин on 11.01.2021.
//

#include "Time.h"
#include <chrono>
#include <list>

namespace Time
{
    namespace
    {
        double _time;
        double _deltaTime;
        std::list<double> deltaTime_history;
    }

    double time()
    {
        return _time;
    }

    double deltaTime()
    {
        return _deltaTime;
    }

    void update()
    {
        std::chrono::duration<double> t = std::chrono::system_clock::now().time_since_epoch();
        _deltaTime = t.count() - _time;
        _time = t.count();

        if(_deltaTime > 10000)
            return;

        if(deltaTime_history.size() < 100)
            deltaTime_history.push_back(_deltaTime);
        else {
            deltaTime_history.push_back(_deltaTime);
            deltaTime_history.pop_front();
        }

    }

    int fps() {
        double sum = 0;
        for (auto dt : deltaTime_history)
            sum += dt;

        return floor((double)deltaTime_history.size() / sum);
    }
}

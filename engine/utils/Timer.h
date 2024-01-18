//
// Created by Иван Ильин on 03.11.2021.
//

#ifndef SHOOTER_TIMER_H
#define SHOOTER_TIMER_H

#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point _startTime;
    std::chrono::high_resolution_clock::time_point _endTime;
    bool _isTicking = false;
    bool _isStopped = true;

    double _elapsedSeconds = 0;
public:
    void start();
    void stop();
    void pause();

    [[nodiscard]] double elapsedMilliseconds() const;
    [[nodiscard]] double elapsedSeconds() const;
};


#endif //SHOOTER_TIMER_H

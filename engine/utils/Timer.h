#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

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


#endif //UTILS_TIMER_H

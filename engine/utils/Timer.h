#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

#include <chrono>

/**
 * @class Timer
 * @brief A utility class for measuring time intervals.
 *
 * This class provides a simple interface for timing operations in C++. It uses the high_resolution_clock
 * from the <chrono> library to measure time intervals with high precision. The Timer can be started, stopped,
 * and paused, allowing for flexible time measurement in applications.
 *
 * @private_section
 * _startTime: The start point of the timing interval.
 * _endTime: The end point of the timing interval.
 * _isTicking: Flag indicating whether the timer is currently running.
 * _isStopped: Flag indicating whether the timer has been stopped.
 * _elapsedSeconds: Stores the total elapsed time in seconds.
 *
 * @public_section
 * start(): Begins or resumes timing from the current _startTime.
 * stop(): Stops the timer and calculates the total elapsed time.
 * pause(): Pauses the timer, allowing it to be resumed later.
 * elapsedMilliseconds(): Returns the total elapsed time in milliseconds.
 * elapsedSeconds(): Returns the total elapsed time in seconds.
 */
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

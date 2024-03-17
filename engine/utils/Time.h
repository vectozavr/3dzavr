#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <chrono>
#include <map>
#include <optional>
#include <functional>
#include <string>

#include <utils/Timer.h>

class Time final {
private:
    std::map<std::string, Timer> _timers;

    // High precision time
    std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point _last = _start;

    // FPS counter
    std::chrono::high_resolution_clock::time_point _fpsStart{};
    std::chrono::milliseconds _fpsCountTime = std::chrono::milliseconds(1000);
    int _fpsCounter = 0;
    double _lastFps = 0;

    // Compatibility
    double _time = 0;
    double _deltaTime = 0;

    static Time *_instance;

    Time() = default;

public:
    Time(const Time &) = delete;

    Time &operator=(Time &) = delete;

    static int fps();

    static double time();

    static double deltaTime();

    static void update();

    static void init();

    static void free();

    static void startTimer(const std::string& timerName);
    static void stopTimer(const std::string& timerName);
    static void pauseTimer(const std::string& timerName);
    [[nodiscard]] static double elapsedTimerMilliseconds(const std::string& timerName);
    [[nodiscard]] static double elapsedTimerSeconds(const std::string& timerName);
    [[nodiscard]] static std::optional<std::reference_wrapper<const std::map<std::string, Timer>>> timers();

    static std::string getLocalTimeInfo(const std::string& format = "%F %T");
};

#endif //UTILS_TIME_H

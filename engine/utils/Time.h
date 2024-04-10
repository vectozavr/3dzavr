#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <chrono>
#include <map>
#include <optional>
#include <functional>
#include <string>

#include <utils/Timer.h>
#include <Consts.h>

class Time final {
private:
    std::map<std::string, Timer> _timers;

    // High precision time
    std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point _last = _start;

    // FPS counter
    std::chrono::high_resolution_clock::time_point _fpsStart{};
    std::chrono::milliseconds _fpsCountTime = std::chrono::milliseconds(500);
    int _fpsCounter = 0;
    unsigned int _lastFps = 0;

    double _time = 0;
    double _deltaTime = 0;
    unsigned int _frame = 0;
    double _fixedDeltaTime = Consts::FIXED_UPDATE_INTERVAL;

    static Time *_instance;

    Time() = default;

public:
    Time(const Time &) = delete;

    Time &operator=(Time &) = delete;

    static void update();
    static void init();
    static void free();

    static void startTimer(const std::string& timerName);
    static void stopTimer(const std::string& timerName);
    static void pauseTimer(const std::string& timerName);

    static void setFixedUpdateInterval(double fixedDeltaTime);

    [[nodiscard]] static unsigned int fps();
    [[nodiscard]] static double time();
    [[nodiscard]] static unsigned int frame();
    [[nodiscard]] static double deltaTime();
    [[nodiscard]] static double fixedDeltaTime();
    [[nodiscard]] static double elapsedTimerMilliseconds(const std::string& timerName);
    [[nodiscard]] static double elapsedTimerSeconds(const std::string& timerName);
    [[nodiscard]] static std::optional<std::reference_wrapper<const std::map<std::string, Timer>>> timers();

    [[nodiscard]] static std::string getLocalTimeInfo(const std::string& format = "%F %T");
};

#endif //UTILS_TIME_H

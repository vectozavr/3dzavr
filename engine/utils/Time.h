#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <chrono>
#include <map>
#include <optional>
#include <functional>
#include <string>

#include <utils/Timer.h>
#include <Consts.h>

/**
 * @class Time
 * @brief Defines the Singleton Time class that manages time-related functionalities such as timers, frame counting, and FPS calculation.
 *
 * This class is designed to handle various time-related tasks in an application, including managing multiple timers,
 * calculating delta time between frames, and keeping track of the frames per second (FPS). It uses high-resolution
 * clocks to ensure precise time measurement. The class is implemented as a singleton to provide a global point of
 * access to time functionalities.
 */
class Time final {
private:
    std::map<std::string, Timer> _timers; ///< A map of string identifiers to Timer objects, allowing for multiple named timers to be managed.

    std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now(); ///< The starting point of the application's runtime, used for calculating total runtime.
    std::chrono::high_resolution_clock::time_point _last = _start; ///< The end point of the last frame, used for calculating delta time.

    std::chrono::high_resolution_clock::time_point _fpsStart{}; ///< The starting point for calculating FPS over a fixed interval.
    std::chrono::milliseconds _fpsCountTime = std::chrono::milliseconds(500); ///< The duration of the interval over which FPS is calculated.
    int _fpsCounter = 0; ///< The number of frames counted within the current FPS interval.
    unsigned int _lastFps = 0; ///< The last calculated FPS value.

    double _time = 0; ///< The total runtime of the application since start.
    double _deltaTime = 0; ///< The time elapsed between the current frame and the last frame.
    unsigned int _frame = 0; ///< The current frame count since the start of the application.
    double _fixedDeltaTime = Consts::FIXED_UPDATE_INTERVAL; ///< A fixed time step value for use in fixed update loops.

    static Time *_instance; ///< A static pointer to the singleton instance of the Time class.

    Time() = default; ///< The constructor is private to prevent instantiation from outside the class.

public:
    Time(const Time &) = delete;
    Time &operator=(Time &) = delete;

    /**
     * @brief Updates the time-related calculations, such as delta time and FPS.
     */
    static void update();

    /**
     * @brief Initializes the Time system, setting up the start time and other initial values.
     */
    static void init();

    /**
     * @brief Cleans up resources used by the Time system, if necessary.
     */
    static void free();

    /**
     * @brief Starts or resumes a named timer.
     *
     * @param timerName The name of the timer to start or resume.
     */
    static void startTimer(const std::string& timerName);

    /**
     * @brief Stops a named timer and calculates its elapsed time.
     *
     * @param timerName The name of the timer to stop.
     */
    static void stopTimer(const std::string& timerName);

    /**
     * @brief Pauses a named timer without resetting its elapsed time.
     *
     * @param timerName The name of the timer to pause.
     */
    static void pauseTimer(const std::string& timerName);

    /**
     * @brief Sets the interval for fixed updates, affecting the fixedDeltaTime value.
     *
     * @param fixedDeltaTime The interval for fixed updates in seconds.
     */
    static void setFixedUpdateInterval(double fixedDeltaTime);

    /**
     * @brief Returns the last calculated FPS value.
     *
     * @return The last calculated FPS value.
     */
    [[nodiscard]] static unsigned int fps();

    /**
     * @brief Returns the total runtime of the application since start.
     *
     * @return The total runtime in seconds.
     */
    [[nodiscard]] static double time();

    /**
     * @brief Returns the current frame count.
     *
     * @return The current frame count since the start of the application.
     */
    [[nodiscard]] static unsigned int frame();

    /**
     * @brief Returns the time elapsed between the current frame and the last frame.
     *
     * @return The delta time in seconds.
     */
    [[nodiscard]] static double deltaTime();

    /**
     * @brief Returns the fixed time step value.
     *
     * @return The fixed time step value in seconds.
     */
    [[nodiscard]] static double fixedDeltaTime();

    /**
     * @brief Returns the elapsed time in milliseconds for a named timer.
     *
     * @param timerName The name of the timer.
     * @return The elapsed time in milliseconds.
     */
    [[nodiscard]] static double elapsedTimerMilliseconds(const std::string& timerName);

    /**
     * @brief Returns the elapsed time in milliseconds for a named timer.
     *
     * @param timerName The name of the timer.
     * @return The elapsed time in milliseconds.
     */
    [[nodiscard]] static double elapsedTimerSeconds(const std::string& timerName);

    /**
     * @brief Returns a read-only reference to the map of timers.
     *
     * This function provides access to the internal map of timers, allowing for read-only operations. It is useful for inspecting the current state of all timers managed by the Time class.
     *
     * @return An optional containing a reference to the map of timers if any exist, or an empty optional if no timers are currently managed.
     */
    [[nodiscard]] static std::optional<std::reference_wrapper<const std::map<std::string, Timer>>> timers();

    /**
     * @brief Retrieves the current local time formatted according to the specified format string.
     *
     * This static method returns a string representing the local time, formatted according to the provided format string.
     * The format string follows the same specifications as used by strftime. For example, "%F %T" would return the date
     * and time in the format "YYYY-MM-DD HH:MM:SS". This method is useful for logging, debugging, or displaying the current
     * time in a user-friendly format.
     *
     * @param format A string specifying the format of the output time string. Defaults to "%F %T" (ISO 8601 date followed by time).
     * @return A string representing the local time formatted according to the specified format string.
     */
    [[nodiscard]] static std::string getLocalTimeInfo(const std::string& format = "%F %T");
};

#endif //UTILS_TIME_H

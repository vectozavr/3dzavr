#include <cstdint>

#ifndef ENGINE_SCALAR_CONSTS_H
#define ENGINE_SCALAR_CONSTS_H

namespace Consts {
    constexpr uint16_t STANDARD_SCREEN_WIDTH = 640;
    constexpr uint16_t STANDARD_SCREEN_HEIGHT = 480;
    constexpr uint16_t SCREEN_SCALE = 2;

    constexpr bool USE_LOG_FILE = true;
    constexpr bool SHOW_DEBUG_INFO = false;

    constexpr double PI = 3.14159265358979323846264338327950288;
    constexpr double EPS = 0.00000000001;

    constexpr double EPA_EPS = 0.0001;

    constexpr double LARGEST_TIME_STEP = 1.0 / 15.0;
    constexpr double TAP_DELAY = 0.2;
}

#endif //ENGINE_SCALAR_CONSTS_H

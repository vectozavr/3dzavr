#ifndef ENGINE_SCALAR_CONSTS_H
#define ENGINE_SCALAR_CONSTS_H

#include <cstdint>

namespace Consts {
    constexpr uint16_t STANDARD_SCREEN_WIDTH = 640;
    constexpr uint16_t STANDARD_SCREEN_HEIGHT = 480;

    constexpr uint16_t STANDARD_EDITOR_WIDTH = STANDARD_SCREEN_WIDTH + 300;
    constexpr uint16_t STANDARD_EDITOR_HEIGHT = STANDARD_SCREEN_HEIGHT;
    constexpr uint16_t SCREEN_SCALE = 1;

    constexpr bool USE_LOG_FILE = true;
    constexpr bool SHOW_DEBUG_INFO = false;

    constexpr double PI = 3.14159265358979323846264338327950288;
    constexpr double EPS = 0.00000000001;
    constexpr double ABG_TRIANGLE_BORDER_WIDTH = 0.01;

    constexpr double LIGHTING_LOD_NEAR_DISTANCE = 5;
    constexpr double LIGHTING_LOD_FAR_DISTANCE = 10;

    constexpr double EPA_EPS = 0.0001;

    constexpr double LARGEST_TIME_STEP = 1.0 / 15.0;
    constexpr double TAP_DELAY = 0.2;

    constexpr int MB = 1024*1024;
}

#endif //ENGINE_SCALAR_CONSTS_H

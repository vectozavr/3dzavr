#ifndef ENGINE_SCALAR_CONSTS_H
#define ENGINE_SCALAR_CONSTS_H

#include <cstdint>

namespace Consts {
    constexpr uint16_t STANDARD_SCREEN_WIDTH = 960; //  640 | 960
    constexpr uint16_t STANDARD_SCREEN_HEIGHT = 720; // 480 | 720

    constexpr uint16_t STANDARD_EDITOR_WIDTH = STANDARD_SCREEN_WIDTH + 300;
    constexpr uint16_t STANDARD_EDITOR_HEIGHT = STANDARD_SCREEN_HEIGHT;
    constexpr uint16_t SCREEN_SCALE = 1;

    constexpr bool USE_LOG_FILE = true;
    constexpr bool SHOW_DEBUG_INFO = false;

    constexpr double PI = 3.14159265358979323846264338327950288;
    constexpr double EPS = 0.00000000001; // 1e-11
    constexpr double ABG_TRIANGLE_BORDER_WIDTH = 0.01;

    constexpr double LIGHTING_LOD_NEAR_DISTANCE = 5;
    constexpr double LIGHTING_LOD_FAR_DISTANCE = 10;

    constexpr unsigned int GJK_MAX_ITERATIONS = 30;
    constexpr unsigned int EPA_MAX_ITERATIONS = 30;
    constexpr double EPA_DEPTH_EPS = 0.0001; // 1e-4
    constexpr double EPA_CONTACT_PLANE_DISTANCE_EPS = 0.01; // 1e-4


    constexpr double FIXED_UPDATE_INTERVAL = 0.02; // 50 fps
    constexpr double TAP_DELAY = 0.2;

    constexpr int MB = 1024*1024;
}

#endif //ENGINE_SCALAR_CONSTS_H

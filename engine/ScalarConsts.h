#ifndef ENGINE_SCALAR_CONSTS_H
#define ENGINE_SCALAR_CONSTS_H

#include <cstdint>

namespace Consts {
    // Standard screen width in pixels (alternative values: 640 or 960).
    constexpr uint16_t STANDARD_SCREEN_WIDTH = 640;
    // Standard screen height in pixels (alternative values: 480 or 720).
    constexpr uint16_t STANDARD_SCREEN_HEIGHT = 480;

    // Width of the editor screen, calculated by adding 300 pixels to the standard screen width.
    constexpr uint16_t STANDARD_EDITOR_WIDTH = STANDARD_SCREEN_WIDTH + 300;
    // Height of the editor screen, equal to the standard screen height.
    constexpr uint16_t STANDARD_EDITOR_HEIGHT = STANDARD_SCREEN_HEIGHT;
    // Scale factor for the screen, currently set to 1 (no scaling).
    constexpr uint16_t SCREEN_SCALE = 1;

    // Flag to determine whether logging to a file is enabled.
    constexpr bool USE_LOG_FILE = true;
    // Flag to control the visibility of debug information on the screen.
    constexpr bool SHOW_DEBUG_INFO = false;

    // Mathematical constant Pi, defined to high precision.
    constexpr double PI = 3.14159265358979323846264338327950288;
    // Small epsilon value for floating-point comparisons, set to 1e-11.
    constexpr double EPS = 0.00000000001;
    // Default border width for ABG triangles, used in rendering.
    constexpr double ABG_TRIANGLE_BORDER_WIDTH = 0.01;

    // Near distance for level-of-detail (LOD) calculations in lighting, set to 5 units.
    constexpr double LIGHTING_LOD_NEAR_DISTANCE = 5;
    // Far distance for level-of-detail (LOD) calculations in lighting, set to 10 units.
    constexpr double LIGHTING_LOD_FAR_DISTANCE = 10;

    // Maximum number of iterations for the GJK (Gilbert-Johnson-Keerthi) algorithm.
    constexpr unsigned int GJK_MAX_ITERATIONS = 30;
    // Maximum number of iterations for the EPA (Expanding Polytope Algorithm).
    constexpr unsigned int EPA_MAX_ITERATIONS = 30;
    // Epsilon value for depth calculations in the EPA, set to 1e-4.
    constexpr double EPA_DEPTH_EPS = 0.0001;
    // Epsilon value for contact plane distance calculations in the EPA, also set to 1e-4.
    constexpr double EPA_CONTACT_PLANE_DISTANCE_EPS = 0.0001;

    // Fixed update interval for the game loop, set to 0.02 seconds (50 fps).
    constexpr double FIXED_UPDATE_INTERVAL = 0.02;
    // Maximum delay for recognizing tap inputs, set to 0.2 seconds.
    constexpr double TAP_DELAY = 0.2;

    // Memory size of 1 Megabyte, defined for convenience.
    constexpr int MB = 1024*1024;
}

#endif //ENGINE_SCALAR_CONSTS_H

#ifndef ENGINE_CONSTS_H
#define ENGINE_CONSTS_H

#include <filesystem>

#include <ScalarConsts.h>
#include <objects/props/Material.h>
#include <objects/props/Color.h>
#include <linalg/Vec2D.h>
#include <utils/FilePath.h>

namespace Consts {
    const Color BACKGROUND_COLOR = Color(255, 255, 255);

    // Running mode detection
#ifndef NDEBUG
    const std::string RUNNING_MODE = "Debug";
#else
    const std::string RUNNING_MODE = "Release";
#endif

    // OS Detection
#if defined(_WIN32) || defined(_WIN64)
    const std::string OPERATION_SYSTEM = "Windows";
#elif defined(__linux__)
    const std::string OPERATION_SYSTEM = "Linux";
#elif defined(__APPLE__) && defined(__MACH__)
    const std::string OPERATION_SYSTEM = "MacOS";
#else
    const std::string OPERATION_SYSTEM = "Unknown";
#endif

    // Processor Architecture
#if defined(__x86_64__) || defined(_M_X64)
    const std::string CPU_ARCHITECTURE = "x86-64";
#elif defined(__i386__) || defined(_M_IX86)
    const std::string CPU_ARCHITECTURE = "x86";
#elif defined(__arm__)
    const std::string CPU_ARCHITECTURE = "ARM";
#elif defined(__aarch64__)
    const std::string CPU_ARCHITECTURE = "ARM64";
#else
    const std::string CPU_ARCHITECTURE = "Unknown";
#endif

    const std::string BUILD_INFO = "3dzavr sdl v0.2.0 CPU | " + RUNNING_MODE;

    const Vec2D BEZIER[2] = {Vec2D{0.8, 0}, Vec2D{0.2, 1}};

    const Color WHITE_COLORS[] = {
            Color(137, 135, 222), // blue
            Color(195, 155, 209), // pink
            Color(201, 137, 137), // red
            Color(116, 204, 135), // green
            Color(201, 171, 137), // orange
    };

    const Color DARK_COLORS[] = {
            Color(16, 18, 69), // blue
            Color(77, 0, 62), // pink
            Color(99, 20, 20), // red
            Color(12, 46, 9), // green
            Color(97, 70, 51), // orange
    };

    const Color WHITE = Color(255, 255, 255);
    const Color BLACK = Color(0, 0, 0);
    const Color RED = Color(255, 0, 0);
    const Color GREEN = Color(0, 255, 0);
    const Color BLUE = Color(0, 0, 255);

    const FilePath DEFAULT_TEXTURE_PATH = FilePath("engine/resources/png/texture_test.png");
    const std::shared_ptr<Material> DEFAULT_MATERIAL = std::make_shared<Material>(
            MaterialTag("Default_material"),
            std::make_shared<Texture>(DEFAULT_TEXTURE_PATH),
                    Color(255, 0, 0),
                    Color(255, 0, 0),
                    Color(255, 0, 0));

    // resources
    const FilePath DEFAULT_FONT_FILENAME = FilePath("engine/resources/fonts/Roboto/Roboto-Light.ttf");
}

#endif //ENGINE_CONSTS_H

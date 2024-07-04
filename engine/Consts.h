#ifndef ENGINE_CONSTS_H
#define ENGINE_CONSTS_H

#include <filesystem>

#include <ScalarConsts.h>
#include <components/props/Material.h>
#include <components/props/Color.h>
#include <linalg/Vec2D.h>
#include <utils/FilePath.h>

namespace Consts {
    // Default background color for the application.
    const Color BACKGROUND_COLOR = Color(255, 255, 255);

    // Determines the running mode of the application (Debug/Release) based on the NDEBUG macro.
#ifndef NDEBUG
    const std::string RUNNING_MODE = "Debug";
#else
    const std::string RUNNING_MODE = "Release";
#endif

    // Detects the operating system on which the application is running.
#if defined(_WIN32) || defined(_WIN64)
    const std::string OPERATION_SYSTEM = "Windows";
#elif defined(__linux__)
    const std::string OPERATION_SYSTEM = "Linux";
#elif defined(__APPLE__) && defined(__MACH__)
    const std::string OPERATION_SYSTEM = "MacOS";
#else
    const std::string OPERATION_SYSTEM = "Unknown";
#endif

    // Identifies the CPU architecture of the system running the application.
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

    // Provides build information including the version and the running mode.
    const std::string BUILD_INFO = "3dzavr sdl v0.2.1 CPU | " + RUNNING_MODE;

    // Defines the control points for a Bezier curve used in the application.
    const Vec2D BEZIER[2] = {Vec2D{0.8, 0}, Vec2D{0.2, 1}};

    // Array of predefined white color shades for UI elements or game objects.
    // Each color is represented in RGB format.
    const Color WHITE_COLORS[] = {
            Color(137, 135, 222), // blue
            Color(195, 155, 209), // pink
            Color(201, 137, 137), // red
            Color(116, 204, 135), // green
            Color(201, 171, 137), // orange
    };

    // Array of predefined dark color shades for UI elements or game objects.
    // Each color is represented in RGB format.
    const Color DARK_COLORS[] = {
            Color(16, 18, 69), // blue
            Color(77, 0, 62), // pink
            Color(99, 20, 20), // red
            Color(12, 46, 9), // green
            Color(97, 70, 51), // orange
    };

    // File path for default texture (for testing purposes).
    // Points to a texture file in the PNG format
    const FilePath DEFAULT_TEXTURE_PATH = FilePath("engine/resources/png/texture_test.png");

    // Default material used for rendering objects in the application.
    // This material is initialized with a default tag and a base color.
    const std::shared_ptr<Material> DEFAULT_MATERIAL = std::make_shared<Material>(
            MaterialTag("Default_material"), nullptr,
            Color(255, 200, 170),Color(255, 200, 170),Color(255, 200, 170));

    // Default file path for font resources used in the application.
    // This path points to the Roboto Light font file in the TrueType Font format.
    const FilePath DEFAULT_FONT_FILENAME = FilePath("engine/resources/fonts/Roboto/Roboto-Light.ttf");
}

#endif //ENGINE_CONSTS_H

//
// Created by Иван Ильин on 07.10.2021.
//

#ifndef ENGINE_CONSTS_H
#define ENGINE_CONSTS_H

#include <filesystem>

#include <ScalarConsts.h>
#include <objects/props/Color.h>
#include <linalg/Vec2D.h>
#include <utils/FilePath.h>

namespace Consts {
    const Color BACKGROUND_COLOR = Color(255, 255, 255);
    const std::string PROJECT_NAME = "engine";

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

    // resources
    const FilePath DEFAULT_FONT_FILENAME = FilePath("engine/resources/fonts/Roboto/Roboto-Light.ttf");
}

#endif //ENGINE_CONSTS_H

//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "io/Screen.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "World.h"
#include "Camera.h"
#include "utils/Log.h"

class Engine {
private:
    std::string _name;

    int _triPerSec = 0;
    bool _updateWorld = true;
    bool _showDebugInfo = Consts::SHOW_DEBUG_INFO;
    bool _useOpenGL = Consts::USE_OPEN_GL;

    void printDebugInfo() const;

protected:
    const std::shared_ptr<Screen> screen = std::make_shared<Screen>();
    const std::shared_ptr<Keyboard> keyboard = std::make_shared<Keyboard>();
    const std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>(screen);

    const std::shared_ptr<World> world = std::make_shared<World>();
    const std::shared_ptr<Camera> camera = std::make_shared<Camera>();

    virtual void start() {};

    virtual void update() {};

    [[nodiscard]] bool showDebugInfo() const { return _showDebugInfo; }
    void setDebugInfo(bool value) { _showDebugInfo = value; }

    void setUpdateWorld(bool value) { _updateWorld = value; }

    void setGlEnable(bool value) { _useOpenGL = value; }
    [[nodiscard]] bool glEnable() const { return _useOpenGL; }

    virtual void gui() {}

public:
    Engine();

    virtual ~Engine() = default;

    void create(int screenWidth = Consts::STANDARD_SCREEN_WIDTH, int screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
                const std::string &name = Consts::PROJECT_NAME, bool verticalSync = true,
                sf::Color background = Consts::BACKGROUND_COLOR, sf::Uint32 style = sf::Style::Default);

    void exit();
};


#endif //INC_3DZAVR_TDZAVR_H

//
// Created by Иван Ильин on 26.03.2021.
//

#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <utility>
#include <memory>

#include "Button.h"
#include "../io/Screen.h"
#include "../io/Mouse.h"

class Window final {
private:
    std::string _name;
    std::string _backTexture;
    std::vector<Button> _buttons;

    sf::Sprite _back;

    Vec2D _prevMousePosition{0, 0};

    std::shared_ptr<Screen> _screen;
    std::shared_ptr<Mouse> _mouse;
public:
    explicit Window(std::shared_ptr<Screen> screen, std::shared_ptr<Mouse> mouse, std::string name = "Menu",
                    std::string backTexture = "") : _screen(screen), _mouse(mouse),
                                                    _name(std::move(name)), _backTexture(std::move(backTexture)) {}

    void addButton(int x, int y, int w, int h,
                   std::function<void()> click,
                   const std::string &text = "_button", double sx = 1, double sy = 1,
                   const std::string &texture = "", tPos usualState = {}, tPos selectedState = {},
                   tPos pressedState = {},
                   const std::string &font = Consts::MEDIUM_FONT, sf::Color textColor = {255, 255, 255});

    void setTitle(const std::string &title) { _name = title; }

    void setBackgroundTexture(const std::string &texture, double sx = 1, double sy = 1, int w = 1920, int h = 1080);

    void update();
};


#endif //MINECRAFT_3DZAVR_WINDOW_H

//
// Created by Иван Ильин on 26.03.2021.
//

#ifndef MINECRAFT_3DZAVR_WINDOW_H
#define MINECRAFT_3DZAVR_WINDOW_H

#include <utility>

#include "Button.h"
#include "Screen.h"

class Window {
private:
    std::string s_name;
    std::string s_backTexture;
    std::vector<Button> buttons;

    sf::Sprite back;

    Point4D prevMousePosition;
public:
    explicit Window(std::string name = "Menu", std::string  backTexture = "") : s_name(std::move(name)), s_backTexture(std::move(backTexture)){}

    void addButton(int x, int y, int w, int h,
                   std::function<void()> click,
                   const std::string& text = "button", double sx = 1, double sy = 1,
                   const std::string& texture = "", tPos usualState = {}, tPos selectedState = {}, tPos pressedState = {},
                   const std::string& font = "../tdzavrlib/fonts/Roboto-Medium.ttf", sf::Color textColor = {255, 255, 255}, const std::string& clickSound = "");

    [[nodiscard]] std::string title() const { return s_name; }
    void title(const std::string& title) { s_name = title; }

    void setBackgroundTexture(const std::string& texture, double sx = 1, double sy = 1, int w = 1920, int h = 1080);

    void update(const std::shared_ptr<Screen>& screen);
};


#endif //MINECRAFT_3DZAVR_WINDOW_H

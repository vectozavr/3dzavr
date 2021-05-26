//
// Created by Иван Ильин on 26.03.2021.
//

#ifndef MINECRAFT_3DZAVR_BUTTON_H
#define MINECRAFT_3DZAVR_BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct tPos {
    int tx;
    int ty;
};

struct Button
{
    int x;
    int y;

    int w;
    int h;

    std::function<void()> click;

    std::string s_text;

    double sx;
    double sy;

    std::string s_texture;
    tPos usualState;
    tPos selectedState;
    tPos pressedState;

    std::string s_font;
    sf::Color textColor;

    std::string s_clickSound;

    sf::Sprite button;
    sf::Text text;
    sf::Sound clickSound;

    bool selected = false;
    bool pressed = false;
    bool checkBox = false;

    void select();
    void unSelect();
    void press();

    void init();
};


#endif //MINECRAFT_3DZAVR_BUTTON_H

//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_SCREEN_H
#define INC_3DZAVR_SCREEN_H


#include <string>
#include "Triangle.h"
#include <SFML/Graphics.hpp>

class Screen {
private:
    int w;
    int h;

    std::string name;

    sf::Color background;

public:
    sf::RenderWindow window;

    Screen() = default;

    void open(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "3dzavr", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255));

    void display();
    void clear();

    void line(const Point4D& p1, const Point4D& p2, sf::Color color = {252, 159, 119});
    void triangle(const Triangle& triangle, sf::Color color, bool boundary = false);

    void title(const std::string& title);
    std::string title() const { return name; };

    bool isOpen();

    int width() const {return w;}
    int height() const {return h;}

    void close();

    // keyboard methods
    static bool isKeyPressed(sf::Keyboard::Key key);
    Point4D getMousePosition() const;
    Point4D getMouseDisplacement() const;
    void setMouseInCenter() const;
    void setMouseCursorVisible(bool visible);
};


#endif //INC_3DZAVR_SCREEN_H

//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_SCREEN_H
#define INC_3DZAVR_SCREEN_H


#include <string>
#include "Triangle.h"
#include <SFML/Graphics.hpp>
#include <map>
#include "Time.h"

class Screen {
public:
    enum ViewMode {
        Default = 0,
        Frame,
        Borders,
        Xray,
        Clipped
    };
private:
    int w = 1920;
    int h = 1080;

    std::string name;

    sf::Color background;

    Screen::ViewMode vm = Screen::ViewMode::Default;

    std::map<sf::Keyboard::Key, double> tappedKey;

    sf::Font font;

    bool renderVideo = false; // performance heavy. I use this to make sequence of .jpg files of screen and then convert this to .mp4 file
    int frame = 0;
    int scene = 0; // the number of scene
public:
    sf::RenderWindow window;

    void open(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "3dzavr", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255));

    void display();
    void clear();

    void line(const Point4D& p1, const Point4D& p2, sf::Color color = {0, 0, 0});
    void triangle(const Triangle& triangle );

    void title(const std::string& title);
    std::string title() const { return name; };

    bool isOpen();

    int width() const {return window.getSize().x;}
    int height() const {return window.getSize().y;}

    void close();

    static bool isKeyPressed(sf::Keyboard::Key key); // returns true if this key is pressed
    bool isKeyTapped(sf::Keyboard::Key key); // returns true if this key is tapped and 1/5 sec passed (button bouncing problem solved)

    Point4D getMousePosition() const;
    Point4D getMouseDisplacement() const;
    void setMouseInCenter() const;
    void setMouseCursorVisible(bool visible);

    void setMode(ViewMode mode) { vm = mode; }
    [[nodiscard]] ViewMode mode() const { return vm; }

    void keyboardControl();

    void debugText(const std::string& text);

    __attribute__ ((deprecated)) void setRender(bool r);
    __attribute__ ((deprecated)) bool isRender() const { return renderVideo; }
};


#endif //INC_3DZAVR_SCREEN_H

//
// Created by Иван Ильин on 14.01.2021.
//

#include "Screen.h"
#include "Time.h"

#include <utility>

void Screen::open(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background) {
    this->name = name;
    w = screenWidth;
    h = screenHeight;
    this->background = background;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(w, h), name, sf::Style::Default, settings);
    //window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(verticalSync);
}

void Screen::display() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    std::string title = name + " (" + std::to_string(Time::fps()) + " fps)";
    window.setTitle(title);

    window.display();
}

void Screen::clear() {
    window.clear(background);
}

void Screen::line(const Point4D& p1, const Point4D& p2, sf::Color color)
{
    if (!window.isOpen())
        return;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.x, p1.y), color),
        sf::Vertex(sf::Vector2f(p2.x, p2.y), color)
    };
    window.draw(line, 2, sf::Lines);
}

void Screen::triangle(const Triangle& triangle, sf::Color color, bool boundary)
{
    sf::ConvexShape convex;

    convex.setFillColor(color);
    convex.setPointCount(3);
    if(boundary) {
        // When using this we have significant artefacts on with the small triangles
        //convex.setOutlineThickness(1);
        //convex.setOutlineColor({255, 0, 0});
        // Instead of this we draw 3 lines:
        line(triangle[0], triangle[1]);
        line(triangle[1], triangle[2]);
        line(triangle[2], triangle[0]);
    }

    convex.setPoint(0, sf::Vector2f(triangle[0].x, triangle[0].y));
    convex.setPoint(1, sf::Vector2f(triangle[1].x, triangle[1].y));
    convex.setPoint(2, sf::Vector2f(triangle[2].x, triangle[2].y));

    window.draw(convex);
}

void Screen::title(const std::string& title)
{
    name = title;
}

bool Screen::isOpen() {
    return window.isOpen();
}

void Screen::close() {
    window.close();
}

bool Screen::isKeyPressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

Point4D Screen::getMousePosition() const {
    sf::Vector2<int> pos = sf::Mouse::getPosition(window);
    return Point4D(pos.x, pos.y, 0, 0);
}

Point4D Screen::getMouseDisplacement() const {
    sf::Vector2<int> disp = sf::Mouse::getPosition(window) - sf::Vector2<int>(w/2, h/2);
    setMouseInCenter();
    return Point4D(disp.x, disp.y, 0, 0);
}

void Screen::setMouseInCenter() const {
    sf::Mouse::setPosition({ w / 2, h / 2 }, window);
}

void Screen::setMouseCursorVisible(bool visible) {
    window.setMouseCursorVisible(visible);
}

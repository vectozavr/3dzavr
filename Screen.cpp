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

void Screen::line(const Point4D& p1, const Point4D& p2)
{
    if (!window.isOpen())
        return;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.x, p1.y)),
        sf::Vertex(sf::Vector2f(p2.x, p2.y))
    };

    window.draw(line, 2, sf::Lines);
}

void Screen::triangle(const Triangle& triangle, sf::Color color, bool boundary)
{
    sf::ConvexShape convex;

    convex.setFillColor(color);
    convex.setPointCount(3);
    if(boundary) {
        convex.setOutlineThickness(1);
        convex.setOutlineColor({255, 0, 0});
    }

    convex.setPoint(0, sf::Vector2f(triangle.p1.x, triangle.p1.y));
    convex.setPoint(1, sf::Vector2f(triangle.p2.x, triangle.p2.y));
    convex.setPoint(2, sf::Vector2f(triangle.p3.x, triangle.p3.y));

    window.draw(convex);
}

void Screen::title(const std::string& title)
{
    name = title;
}

bool Screen::isOpen() {
    return window.isOpen();
}

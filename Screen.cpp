//
// Created by Иван Ильин on 14.01.2021.
//

#include "Screen.h"
#include "Time.h"
#include <utility>
#include "Log.h"

void Screen::open(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background) {
    this->name = name;
    w = screenWidth;
    h = screenHeight;
    this->background = background;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(w, h), name, sf::Style::Default, settings);
    //window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(verticalSync);

    if (!font.loadFromFile("../fonts/Roboto-Thin.ttf"))
        Log::log("Screen::open: Cannot load font '../fonts/Roboto-Thin.ttf'");
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

void Screen::triangle(const Triangle& triangle)
{
    if(vm == Frame || vm == Borders || vm == Xray) {
        // Using this we have significant artefacts with small triangles
        //convex.setOutlineThickness(1);
        //convex.setOutlineColor({255, 0, 0});
        // We draw 3 lines instead:
        line(triangle[0], triangle[1]);
        line(triangle[1], triangle[2]);
        line(triangle[2], triangle[0]);
    }
    if(vm == Frame || vm == Xray)
        return; // no texture when we turn on Frame or Xray mode

    sf::ConvexShape convex;

    convex.setFillColor(triangle.color);
    convex.setPointCount(3);

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

void Screen::keyboardControl() {
    if(isKeyTapped(sf::Keyboard::Num1))
        setMode(ViewMode::Default);
    if(isKeyTapped(sf::Keyboard::Num2))
        setMode(ViewMode::Borders);
    if(isKeyTapped(sf::Keyboard::Num3))
        setMode(ViewMode::Frame);
    if(isKeyTapped(sf::Keyboard::Num4))
        setMode(ViewMode::Xray);
}

bool Screen::isKeyTapped(sf::Keyboard::Key key) {
    if (!Screen::isKeyPressed(key))
        return false;

    if(tappedKey.count(key) == 0) {
        tappedKey.emplace(key, Time::time());
        return true;
    } else if((Time::time() - tappedKey[key]) > 0.2) {
        tappedKey[key] = Time::time();
        return true;
    }
    return false;
}

void Screen::debugText(const std::string& text) {
    sf::Text t;

    t.setFont(font);
    t.setString(text);
    t.setCharacterSize(30);
    t.setFillColor(sf::Color::Black);
    //t.setStyle(sf::Text::Bold);
    t.setPosition(10, 10);

    window.draw(t);
}

//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_SCREEN_H
#define ENGINE_SCREEN_H


#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "../Triangle.h"
#include "../utils/Time.h"
#include "../Consts.h"
#include "../Mesh.h"
#include "../Camera.h"

class Screen final {
private:
    int _scene = 0;
    bool _renderVideo = false;
    std::vector<sf::Texture> _renderSequence;

    std::string _title;

    sf::Color _background;

    const std::shared_ptr<sf::RenderWindow> _window = std::make_shared<sf::RenderWindow>();
public:
    void open(int screenWidth = Consts::STANDARD_SCREEN_WIDTH, int screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
              const std::string &name = Consts::PROJECT_NAME, bool verticalSync = true,
              sf::Color background = Consts::BACKGROUND_COLOR, sf::Uint32 style = sf::Style::Default);

    void display();

    void clear();

    [[nodiscard]] bool hasFocus() const { return _window->hasFocus(); }

    void drawTriangle(const Triangle &triangle);

    void drawTetragon(const Vec2D &p1, const Vec2D &p2, const Vec2D &p3, const Vec2D &p4, sf::Color color);

    void drawText(const std::string &string, const Vec2D &position, int size, sf::Color color);

    void drawText(const sf::Text &text);

    void drawSprite(const sf::Sprite &sprite);

    void setTitle(const std::string &title);

    [[nodiscard]] std::string title() const { return _title; };

    bool isOpen();

    [[nodiscard]] int width() const { return _window->getSize().x; }

    [[nodiscard]] int height() const { return _window->getSize().y; }

    void close();

    void setMouseCursorVisible(bool visible);

    // OpenGL functions
    void prepareToGlDrawMesh();
    
    void glDrawMesh(GLfloat *geometry, GLfloat *view, GLfloat *model, size_t count);

    [[nodiscard]] std::shared_ptr<sf::RenderWindow> renderWindow() { return _window; }

    void pushGLStates() { _window->pushGLStates(); };
    void popGLStates() { _window->popGLStates(); };

    void startRender();
    void stopRender();
};


#endif //INC_3DZAVR_SCREEN_H

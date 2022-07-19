//
// Created by Иван Ильин on 14.01.2021.
//

#include <utility>
#include <cmath>

#include <SFML/OpenGL.hpp>

#include "Screen.h"
#include "../utils/Time.h"
#include "../utils/Log.h"
#include "../utils/ResourceManager.h"

void Screen::open(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background,
                  sf::Uint32 style) {
    _title = name;
    _background = background;

    sf::ContextSettings settings;
    settings.depthBits = 12;
    settings.antialiasingLevel = 1;

    _window->create(sf::VideoMode(screenWidth, screenHeight), name, style, settings);
    _window->setVerticalSyncEnabled(verticalSync);
}

void Screen::display() {
    sf::Event event{};
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window->close();
        }
    }

    std::string title = _title + " (" + std::to_string(Time::fps()) + " fps)";
    _window->setTitle(title);

    if(_renderVideo) {
        sf::Texture copyTexture;
        copyTexture.create(_window->getSize().x, _window->getSize().y);
        copyTexture.update(*_window);
        // most of the time of video rendering is wasting on saving .png sequence
        // that's why we will save all images in the end
        // TODO: sometimes we have a huge time delay here for no obvious reason
        _renderSequence.push_back(copyTexture);
    }

    _window->display();
}

void Screen::startRender() {
    stopRender();

    Log::log("Screen::startRender(): start recording the screen");
    _renderVideo = true;
}

void Screen::stopRender() {
    if(_renderVideo) {
        Log::log("Screen::stopRender(): stop recording the screen");
        Log::log("Screen::stopRender(): start saving .png sequence");
        std::string c = "rm film/png/*.png";
        popen(c.c_str(), "w");
        int i = 0;
        for(; i < _renderSequence.size(); i++) {
            _renderSequence[i].copyToImage().saveToFile("film/png/" + std::to_string(i) + ".png");
            Log::log("Screen::stopRender(): saving .png sequence (" + std::to_string(static_cast<int>(100*i/_renderSequence.size())) + "%)");
        }
        _renderSequence.clear();

        Log::log("Screen::stopRender(): start rendering final video");
        // TODO: .png sequence looks better than a final video (poor clarity and desaturated colors)
        c = "ffmpeg -stats -r 60 -i film/png/%d.png -vcodec libx264 -crf 1 -pix_fmt yuv420p -frames " + std::to_string(i) + " film/mp4/" + std::to_string(_scene) + "_" + _title + "_" + std::to_string(rand()) + ".mp4";
        popen(c.c_str(), "w");
        _scene++;
        _renderVideo = false;
        Log::log("Screen::stopRender(): finish rendering final video");
    }
}

void Screen::clear() {
    // Clear the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _window->clear(_background);
}

void Screen::drawTriangle(const Triangle &triangle) {
    sf::Vertex tris[3] =
            {
                    sf::Vertex(sf::Vector2f(static_cast<float>(triangle[0].x()), static_cast<float>(triangle[0].y())),
                               triangle.color()),
                    sf::Vertex(sf::Vector2f(static_cast<float>(triangle[1].x()), static_cast<float>(triangle[1].y())),
                               triangle.color()),
                    sf::Vertex(sf::Vector2f(static_cast<float>(triangle[2].x()), static_cast<float>(triangle[2].y())),
                               triangle.color())
            };

    _window->draw(tris, 3, sf::Triangles);
}

void Screen::setTitle(const std::string &title) {
    _title = title;
}

bool Screen::isOpen() {
    return _window->isOpen();
}

void Screen::close() {
    _window->close();
}

void Screen::setMouseCursorVisible(bool visible) {
    _window->setMouseCursorVisible(visible);
}

void Screen::drawTetragon(const Vec2D &p1, const Vec2D &p2, const Vec2D &p3, const Vec2D &p4, sf::Color color) {
    sf::ConvexShape polygon;
    polygon.setPointCount(4);
    polygon.setPoint(0, sf::Vector2f(static_cast<float>(p1.x()), static_cast<float>(p1.y())));
    polygon.setPoint(1, sf::Vector2f(static_cast<float>(p2.x()), static_cast<float>(p2.y())));
    polygon.setPoint(2, sf::Vector2f(static_cast<float>(p3.x()), static_cast<float>(p3.y())));
    polygon.setPoint(3, sf::Vector2f(static_cast<float>(p4.x()), static_cast<float>(p4.y())));
    polygon.setFillColor(color);

    _window->draw(polygon);
}

void Screen::drawText(const std::string &string, const Vec2D &position, int size, sf::Color color) {
    sf::Text text;

    text.setFont(*ResourceManager::loadFont(Consts::MEDIUM_FONT));

    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setStyle(sf::Text::Italic);
    text.setPosition(static_cast<float>(position.x()), static_cast<float>(position.y()));

    text.setString(string);

    _window->draw(text);
}

void Screen::drawSprite(const sf::Sprite &sprite) {
    _window->draw(sprite);
}

void Screen::drawText(const sf::Text &text) {
    _window->draw(text);
}

// OpenGL functions
void Screen::prepareToGlDrawMesh() {
    glEnable(GL_CULL_FACE); // enable culling face
    glCullFace(GL_BACK); // cull faces from back
    glFrontFace(GL_CCW); // vertex order (counter clock wise)

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glDepthFunc(GL_LESS);

    // Disable lighting
    glDisable(GL_LIGHTING);

    // enable alpha channel:
    //glEnable( GL_ALPHA_TEST );
    //glAlphaFunc(GL_NOTEQUAL, 0.0);
//
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, _window->getSize().x, _window->getSize().y);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(_window->getSize().x) / _window->getSize().y;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.0f, 500.f);

    // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Disable normal and color vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Prepare to apply some transformations
    glMatrixMode(GL_MODELVIEW);
}

// OpenGL functions
void Screen::glDrawMesh(GLfloat* geometry, GLfloat* view, GLfloat* model, size_t count) {
    glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), geometry);
    glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), geometry + 3);

    glLoadIdentity();

    glLoadMatrixf(view);
    glMultMatrixf(model);

    // Draw the mesh
    glDrawArrays(GL_TRIANGLES, 0, count);
}

//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_SCREEN_H
#define ENGINE_SCREEN_H


#include <string>
#include <map>

#include "SDL.h"
#include "SDL_ttf.h"

#include "objects/geometry/Triangle.h"
#include <utils/Time.h>
#include <Consts.h>
#include "objects/geometry/Mesh.h"
#include "objects/Camera.h"

class Screen final {
private:
    SDL_Renderer* _renderer = nullptr;
    SDL_Window* _window = nullptr;

    std::vector<std::vector<float>> _depthBuffer;

    uint16_t _width;
    uint16_t _height;

    int _scene = 0;
    bool _renderVideo = false;

    std::string _title;

    Color _background;

    bool _isOpen = false;

    void initDepthBuffer();

public:
    Screen& operator=(const Screen& scr) = delete;

    void open(uint16_t screenWidth = Consts::STANDARD_SCREEN_WIDTH,
              uint16_t screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
              const std::string &title = Consts::PROJECT_NAME,
              const Color& background = Consts::BACKGROUND_COLOR);
    void display();
    void clear();

    void drawPixel(uint16_t x, uint16_t y, const Color& color); // Without using depth buffer
    void drawPixel(uint16_t x, uint16_t y, float z, const Color& color); // With using depth buffer
    void drawLine(const Vec2D& from, const Vec2D& to, const Color &color, uint16_t thickness = 1);
    void drawTriangle(const Triangle &triangle, std::shared_ptr<Texture> texture = nullptr);
    void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const Color &color);
    void drawStrokeRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                             const Color &color, uint16_t thickness = 1, const Color &strokeColor = Consts::BLACK);

    void drawText(const std::string& text, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fontsize = 12, const Color& color = Consts::BLACK);

    void drawImage(uint16_t x, uint16_t y, std::shared_ptr<Image> img);

    void setTitle(const std::string &title);

    [[nodiscard]] std::string title() const { return _title; };

    [[nodiscard]] bool isOpen() const;

    [[nodiscard]] uint16_t width() const { return _width; }
    [[nodiscard]] uint16_t height() const { return _height; }

    void close();

    void startRender();
    void stopRender();

    void clearDepthBuffer();
};


#endif //INC_3DZAVR_SCREEN_H

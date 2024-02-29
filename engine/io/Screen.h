//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_SCREEN_H
#define ENGINE_SCREEN_H


#include <string>
#include <map>

#include "SDL.h"

#include "objects/geometry/Triangle.h"
#include <utils/Time.h>
#include <Consts.h>
#include "objects/geometry/Mesh.h"
#include "objects/Camera.h"

class Screen final {
private:
    SDL_Renderer* _renderer = nullptr;
    SDL_Window* _window = nullptr;
    SDL_Texture* _screenTexture = nullptr;

    std::vector<float> _depthBuffer;
    std::vector<uint32_t> _pixelBuffer;

    uint16_t _width;
    uint16_t _height;

    int _scene = 0;
    bool _renderVideo = false;

    std::string _title = Consts::BUILD_INFO;

    Color _background;

    bool _isOpen = false;

    void drawPixelUnsafe(uint16_t x, uint16_t y, const Color& color); // Without using depth buffer and checks
    void drawPixelUnsafe(uint16_t x, uint16_t y, double z, const Color &color); // With using depth buffer without checks

public:
    Screen& operator=(const Screen& scr) = delete;

    void open(uint16_t screenWidth = Consts::STANDARD_SCREEN_WIDTH,
              uint16_t screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
              const Color& background = Consts::BACKGROUND_COLOR);
    void display();
    void clear();

    void drawPixel(uint16_t x, uint16_t y, const Color& color); // Without using depth buffer
    void drawPixel(uint16_t x, uint16_t y, double z, const Color& color); // With using depth buffer
    void drawLine(const Vec2D& from, const Vec2D& to, const Color &color, uint16_t thickness = 1);
    void drawTriangle(const Triangle &triangle, Material* material = nullptr);
    void drawTriangle(const Triangle &triangle, const Color &color);
    void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const Color &color);
    void drawStrokeRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                             const Color &color, uint16_t thickness = 1, const Color &strokeColor = Consts::BLACK);

    void drawText(const std::string& text, uint16_t x, uint16_t y, uint16_t fontsize = 12, const Color& color = Consts::BLACK);

    void drawImage(uint16_t x, uint16_t y, std::shared_ptr<Image> img);

    void drawPlot(const std::vector<std::pair<double, double>>& data, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    void setTitle(const std::string &title);

    [[nodiscard]] std::string title() const { return _title; };

    [[nodiscard]] bool isOpen() const;

    [[nodiscard]] uint16_t width() const { return _width; }
    [[nodiscard]] uint16_t height() const { return _height; }

    void close();

    void startRender();
    void stopRender();

    ~Screen();
};


#endif //ENGINE_SCREEN_H

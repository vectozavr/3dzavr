#ifndef IO_SCREEN_H
#define IO_SCREEN_H

#include <string>
#include <map>

#include "SDL.h"

#include <Consts.h>
#include <utils/Font.h>
#include <utils/Time.h>
#include <objects/Camera.h>
#include <objects/geometry/Triangle.h>
#include <objects/geometry/TriangleMesh.h>
#include <objects/lighting/LightSource.h>


class Screen final {
private:
    SDL_Renderer* _renderer = nullptr;
    SDL_Window* _window = nullptr;
    SDL_Texture* _screenTexture = nullptr;

    std::vector<double> _depthBuffer;
    std::vector<uint32_t> _pixelBuffer;

    uint16_t _width;
    uint16_t _height;
    bool _depthTest = false;

    bool _renderVideo = false;
    double _lastFrameTime = 0;
    int _clipFps = 30;
    int _clipCrf = 28;
    FILE* _ffmpeg = nullptr;

    std::string _title = Consts::BUILD_INFO;

    Color _background;

    bool _isOpen = false;

    bool _enableLighting = true;
    bool _enableTrueLighting = false;
    bool _enableTransparency = true;
    bool _enableTriangleBorders = false;
    bool _enableTexturing = true;
    bool _enableMipmapping = true;

    double _lightingLODNearDistance = Consts::LIGHTING_LOD_NEAR_DISTANCE;
    double _lightingLODFarDistance = Consts::LIGHTING_LOD_FAR_DISTANCE;

    // returns true if z is smaller than what is stored in the _depthBuffer
    [[nodiscard]] bool checkPixelDepth(uint16_t x, uint16_t y, double z) const;

    void drawPixelUnsafe(uint16_t x, uint16_t y, const Color& color); // Without using depth buffer and checks
    void drawPixelUnsafe(uint16_t x, uint16_t y, double z, const Color &color); // With using depth buffer without checks

    void plotLineLow(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness);
    void plotLineHigh(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness);

    void plotLineLow(const Vec3D& from, const Vec3D& to, const Color &color, uint16_t thickness);
    void plotLineHigh(const Vec3D& from, const Vec3D& to, const Color &color, uint16_t thickness);

    void drawLine(const Vec2D& from, const Vec2D& to, const Color &color, uint16_t thickness = 1);

public:
    Screen& operator=(const Screen& scr) = delete;

    void open(uint16_t screenWidth = Consts::STANDARD_SCREEN_WIDTH,
              uint16_t screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
              const Color& background = Consts::BACKGROUND_COLOR);
    void display();
    void clear();

    void drawPixel(int x, int y, const Color& color); // Without using depth buffer
    void drawPixel(int x, int y, double z, const Color& color); // With using depth buffer
    void drawLine(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness = 1);
    void drawLine(const Line& line, const Color &color, uint16_t thickness = 1);
    void drawTriangle(const Triangle &triangle, Material* material = nullptr);
    void drawTriangle(const Triangle &triangle, const Color &color);
    void drawRectangle(int x, int y, uint16_t width, uint16_t height, const Color &color);
    void drawRectangle(int x, int y, uint16_t width, uint16_t height, Material* material = nullptr);
    void drawCircle(int x, int y, uint16_t r, const Color &fillColor);
    void drawCircle(int x, int y, double z, uint16_t r, const Color &fillColor);
    void drawStrokeRectangle(int x, int y, uint16_t width, uint16_t height,
                             const Color &color, uint16_t thickness = 1, const Color &strokeColor = Color::BLACK);
    void drawText(const std::string& text, int x, int y, const Color& color = Color::BLACK, uint16_t fontsize = 12, const std::shared_ptr<Font>& font = nullptr);
    void drawImage(int x, int y, std::shared_ptr<Image> img);
    void drawPlot(const std::vector<std::pair<double, double>>& data, int x, int y, uint16_t w, uint16_t h);

    void drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                  const std::vector<std::shared_ptr<LightSource>>& lights,
                                  const Vec3D& cameraPosition, Material* material = nullptr);
    void drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                  const std::vector<std::shared_ptr<LightSource>>& lights,
                                  const Vec3D& cameraPosition, const Color &color);

    void setTitle(const std::string &title);
    void setDepthTest(bool enable) { _depthTest = enable; };

    void setLighting(bool enable) { _enableLighting = enable;}
    void setTrueLighting(bool enable) { _enableTrueLighting = enable; }
    void setTransparency(bool enable) { _enableTransparency = enable; }
    void setTriangleBorders(bool enable) { _enableTriangleBorders = enable; }
    void setTexturing(bool enable) { _enableTexturing = enable; }
    void setMipmapping(bool enable) { _enableMipmapping = enable; }
    void setLightingLODNearDistance(double distance) { _lightingLODNearDistance = distance; }
    void setLightingLODFarDistance(double distance) { _lightingLODFarDistance = distance; }

    [[nodiscard]] std::string title() const { return _title; };
    [[nodiscard]] bool isOpen() const;
    [[nodiscard]] uint16_t width() const { return _width; }
    [[nodiscard]] uint16_t height() const { return _height; }

    void close();

    void startRender();
    void stopRender();
    void setClipFps(int fps) { _clipFps = fps; }
    void setClipCrf(int crf) { _clipCrf = crf; }
    Image makeScreenShot();

    ~Screen();
};


#endif //IO_SCREEN_H

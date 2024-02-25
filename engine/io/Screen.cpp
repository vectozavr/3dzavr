//
// Created by Иван Ильин on 14.01.2021.
//

#include <algorithm>
#include <utility>
#include <cmath>

#include "SDL.h"

#include <io/Screen.h>
#include <utils/Time.h>
#include <utils/Log.h>
#include <utils/stack_vector.h>
#include <utils/ResourceManager.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define popen _popen
#endif


void Screen::open(uint16_t screenWidth, uint16_t screenHeight, const std::string &name, const Color& background) {
    _title = name;
    _background = background;
    _width = screenWidth;
    _height = screenHeight;

    _isOpen = true;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(_width*Consts::SCREEN_SCALE, _height*Consts::SCREEN_SCALE, 0, &_window, &_renderer);
    SDL_RenderSetLogicalSize(_renderer, _width, _height);

    SDL_SetRenderDrawColor(_renderer, background.r(), background.g(), background.b(), background.a());
    SDL_RenderClear(_renderer);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);

    _screenTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);
    _pixelBuffer.resize(_width * _height);
    _depthBuffer.resize(_width * _height);

    // Initialize SDL_ttf
    if ( TTF_Init() < 0 ) {
        Log::log("Screen::open(): error initializing SDL_ttf: " + std::string(TTF_GetError()));
    }
}

void Screen::display() {

    std::string title = _title + " (" + std::to_string(Time::fps()) + " fps)";

    if(_renderVideo) {
        // most of the time of video rendering is wasting on saving .png sequence
        // that's why we will save all images in the end

        // TODO: implement saving screen state into _renderSequence
        //_renderSequence.push_back(screenState);
    }

    if(_isOpen) {
        SDL_UpdateTexture(_screenTexture, NULL, _pixelBuffer.data(), _width * 4);
        SDL_RenderCopy(_renderer, _screenTexture, NULL, NULL);
        SDL_RenderPresent(_renderer);
    }
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
        /*
        for(; i < _renderSequence.size(); i++) {
            _renderSequence[i].copyToImage().saveToFile("film/png/" + std::to_string(i) + ".png");
            Log::log("Screen::stopRender(): saving .png sequence (" + std::to_string(static_cast<int>(100*i/_renderSequence.size())) + "%)");
        }
        _renderSequence.clear();
        */
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
    std::fill(_depthBuffer.begin(), _depthBuffer.end(), 1.0f);
    std::fill(_pixelBuffer.begin(), _pixelBuffer.end(), 0xFFFFFFFFU);
}

void Screen::drawPixel(const uint16_t x, const uint16_t y, const Color &color) {
    if(x >= _width || x < 0 || y >= _height || y < 0)
        return;

    _pixelBuffer[y * _width + x] = color.rgba();
}

void Screen::drawPixel(uint16_t x, uint16_t y, double z, const Color &color) {
    if(x >= _width || x < 0 || y >= _height || y < 0)
        return;

    if(z < _depthBuffer[y * _width + x]) {
        drawPixelUnsafe(x, y, color);
        _depthBuffer[y * _width + x] = z;
    }
}

inline void Screen::drawPixelUnsafe(const uint16_t x, const uint16_t y, const Color &color) {
    _pixelBuffer[y * _width + x] = color.rgba();
}

inline void Screen::drawPixelUnsafe(uint16_t x, uint16_t y, double z, const Color &color) {
    if (z < _depthBuffer[y * _width + x]) {
        drawPixelUnsafe(x, y, color);
        _depthBuffer[y * _width + x] = z;
    }
}

void Screen::drawLine(const Vec2D& from, const Vec2D& to, const Color &color, uint16_t thickness) {
    // TODO: It is better to use built in SDL functions
    //SDL_SetRenderDrawColor(_renderer, color.r(), color.g(), color.b(), color.a());
    //SDL_RenderDrawLine(_renderer, (int)from.x(), (int)from.y(), (int)to.x(), (int)to.y());

    if(to.x() < from.x()) {
        return drawLine(to, from, color, thickness);
    }

    double der = (to.y() - from.y())/(to.x() - from.x());

    Vec2D from_to = (to - from).normalized();
    Vec2D norm(-from_to.y(), from_to.x());


    auto line = [from, to](double x_i, double y_i){
        return x_i*(to.y() - from.y()) + y_i*(from.x() - to.x()) + to.x()*from.y() - from.x()*to.y();
    };

    auto x = (int)from.x();
    auto y = (int)from.y();
    auto to_x = (int)to.x();
    auto to_y = (int)to.y();

    int step_y = to.y() - from.y() > 0 ? 1 : -1;
    int sign = std::abs(der) <= 1 ? 1 : -1;

    while (x < to_x || y*step_y < to_y*step_y) {
        for(int l = 0; l < thickness; l++) {
            drawPixel(x + norm.x()*l, y + norm.y()*l, color);
        }

        if(line(x, y)*step_y*sign > 0) {
            if(std::abs(der) <= 1) {
                y += step_y;
            } else {
                x += 1;
            }
        }
        if(std::abs(der) <= 1) {
            x += 1;
        } else {
            y += step_y;
        }
    }

    drawPixel(to_x, to_y, color);
}

inline bool isGoodAbg(const Vec3D& abg) {
    return abg.x() >= 0 && abg.y() >= 0 && abg.z() >= 0;
}

bool lineLimits(const Vec3D& abg, const Vec3D& abg_dx, int x_min, int x_max, int& x_line_min, int& x_line_max) {
    stack_vector<double, 4> offsets = stack_vector<double, 4>();
    if (isGoodAbg(abg)) {
        offsets.push_back(0);
    }
    double modifier;
    modifier = -abg.x() / abg_dx.x();
    if (!std::isnan(modifier) && modifier > 0) {
        offsets.push_back(modifier);
    }
    modifier = -abg.y() / abg_dx.y();
    if (!std::isnan(modifier) && modifier > 0) {
        offsets.push_back(modifier);
    }
    modifier = -abg.z() / abg_dx.z();
    if (!std::isnan(modifier) && modifier > 0) {
        offsets.push_back(modifier);
    }

    std::sort(offsets.begin(), offsets.end(), std::greater{});

    while (!offsets.empty() && !isGoodAbg(abg + abg_dx * ceil(offsets.back()))) offsets.pop_back();
    if (offsets.empty()) return false;
    x_line_min = std::clamp<int>(x_min + std::ceil(offsets.back()), x_min, x_max);
    offsets.pop_back();

    while (!offsets.empty() && !isGoodAbg(abg + abg_dx * floor(offsets.back()))) offsets.pop_back();
    if (offsets.empty()) {
        x_line_max = x_line_min;
    } else {
        x_line_max = std::clamp<int>(x_min + std::floor(offsets.back()), x_min, x_max);
    }
    return true;
}

void Screen::drawTriangle(const Triangle &triangle, Material *material) {
    // Drawing edge
    //drawLine(Vec2D(triangle[0]), Vec2D(triangle[1]), Consts::BLACK);
    //drawLine(Vec2D(triangle[1]), Vec2D(triangle[2]), Consts::BLACK);
    //drawLine(Vec2D(triangle[2]), Vec2D(triangle[0]), Consts::BLACK);

    // Filling inside
    int x_min = std::clamp<int>(std::ceil(std::min({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    int y_min = std::clamp<int>(std::ceil(std::min({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);
    int x_max = std::clamp<int>(std::floor(std::max({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    int y_max = std::clamp<int>(std::floor(std::max({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

    auto tc = triangle.textureCoordinates();
    auto texture = material ? material->texture() : nullptr;
    Color color = material ? material->ambient() : Consts::RED;

    // calculate for start coords to increase precision
    Vec3D abg_origin = triangle.abgBarycCoord(Vec2D(x_min, y_min));
    Vec3D abg_dx = triangle.abgBarycCoord(Vec2D(x_min + 1, y_min));
    Vec3D abg_dy = triangle.abgBarycCoord(Vec2D(x_min, y_min + 1));
    abg_dx = abg_dx - abg_origin;
    abg_dy = abg_dy - abg_origin;

    Vec3D uv_hom_origin, uv_hom_dx, uv_hom_dy;

    if (texture) {
        uv_hom_origin = tc[0] + (tc[1] - tc[0]) * abg_origin.y() + (tc[2] - tc[0]) * abg_origin.z();

        // Derivative in X
        Vec3D abg_xp = abg_origin + abg_dx;
        Vec3D uv_hom_xp = tc[0] + (tc[1] - tc[0]) * abg_xp.y() + (tc[2] - tc[0]) * abg_xp.z();
        uv_hom_dx = uv_hom_xp - uv_hom_origin;

        // Derivative in Y
        Vec3D abg_yp = abg_origin + abg_dy;
        Vec3D uv_hom_yp = tc[0] + (tc[1] - tc[0]) * abg_yp.y() + (tc[2] - tc[0]) * abg_yp.z();
        uv_hom_dy = uv_hom_yp - uv_hom_origin;
    }

    for (int y = y_min; y <= y_max; y++) {
        Vec3D abg = abg_origin;
        Vec3D uv_hom = uv_hom_origin;
        abg_origin += abg_dy;
        uv_hom_origin += uv_hom_dy;

        int x_cur_min, x_cur_max;
        if (!lineLimits(abg, abg_dx, x_min, x_max, x_cur_min, x_cur_max)) continue;

        abg += abg_dx * (x_cur_min - x_min);
        uv_hom += uv_hom_dx * (x_cur_min - x_min);

        for (int x = x_cur_min; x <= x_cur_max; x++) {
            if (texture) {
                // de-homogenize UV coordinates
                Vec2D uv_dehom(uv_hom.x() / uv_hom.z(), uv_hom.y() / uv_hom.z());

                // TODO: move calculations of derivatives somewhere from here: it becomes messy
                // Derivative in X
                Vec3D uv_hom_xp = uv_hom + uv_hom_dx;
                Vec2D uv_dehom_xp = Vec2D(uv_hom_xp.x() / uv_hom_xp.z(), uv_hom_xp.y() / uv_hom_xp.z()) - uv_dehom;

                // Derivative in Y
                Vec3D uv_hom_yp = uv_hom + uv_hom_dy;
                Vec2D uv_dehom_yp = Vec2D(uv_hom_yp.x() / uv_hom_yp.z(), uv_hom_yp.y() / uv_hom_yp.z()) - uv_dehom;

                // Derivative in XY combined [Jacobian]
                Vec2D du(texture->width() * uv_dehom_xp.x(), texture->width() * uv_dehom_yp.x());
                Vec2D dv(texture->height() * uv_dehom_xp.y(), texture->height() * uv_dehom_yp.y());

                double area = du.abs() + dv.abs();

                color = texture->get_pixel_from_UV(uv_dehom, area);
            }
            double z = triangle[0].z() * abg.x() + triangle[1].z() * abg.y() + triangle[2].z() * abg.z();
            drawPixelUnsafe(x, y, z, color);
            abg += abg_dx;
            uv_hom += uv_hom_dx;
        }
    }
}

void Screen::drawTriangle(const Triangle &triangle, const Color &color) {
    // Filling inside
    int x_min = std::clamp<int>(std::ceil(std::min({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    int y_min = std::clamp<int>(std::ceil(std::min({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);
    int x_max = std::clamp<int>(std::floor(std::max({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    int y_max = std::clamp<int>(std::floor(std::max({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

    Vec3D abg_origin = triangle.abgBarycCoord(Vec2D(x_min, y_min));
    Vec3D abg_dx = triangle.abgBarycCoord(Vec2D(x_min + 1, y_min));
    Vec3D abg_dy = triangle.abgBarycCoord(Vec2D(x_min, y_min + 1));
    abg_dx = abg_dx - abg_origin;
    abg_dy = abg_dy - abg_origin;

    for (int y = y_min; y <= y_max; y++) {
        Vec3D abg = abg_origin;
        abg_origin += abg_dy;

        int x_cur_min, x_cur_max;
        if (!lineLimits(abg, abg_dx, x_min, x_max, x_cur_min, x_cur_max)) continue;

        for (int x = x_cur_min; x <= x_cur_max; x++) {
            double z = triangle[0].z() * abg.x() + triangle[1].z() * abg.y() + triangle[2].z() * abg.z();
            drawPixelUnsafe(x, y, z, color);
            abg += abg_dx;
        }
    }
}

void Screen::setTitle(const std::string &title) {
    _title = title;
    SDL_SetWindowTitle(_window, title.c_str());
}

bool Screen::isOpen() const {
    return _isOpen;
}

void Screen::drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const Color &color) {
    drawTriangle(Triangle({Vec4D(x, y), Vec4D(x+width, y), Vec4D(x, y+height)}), color);
    drawTriangle(Triangle({Vec4D(x, y+height), Vec4D(x+width, y), Vec4D(x+width, y+height)}), color);
}

void Screen::drawStrokeRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const Color &color,
                                 uint16_t thickness, const Color &strokeColor) {

    drawRectangle(x, y, width, height, color);
    drawLine(Vec2D(x, y), Vec2D(x + width, y), strokeColor);
    drawLine(Vec2D(x+ width, y), Vec2D(x + width, y + height), strokeColor);
    drawLine(Vec2D(x + width, y + height), Vec2D(x, y + height), strokeColor);
    drawLine(Vec2D(x, y + height), Vec2D(x, y), strokeColor);
}

void Screen::drawImage(uint16_t x, uint16_t y, std::shared_ptr<Image> img) {
    for(int i = 0; i < img->width(); i++) {
        for(int j = 0; j < img->height(); j++) {
            drawPixel(i + x, j + y, img->get_pixel(i, j));
        }
    }
}

void Screen::drawText(const std::string& text, uint16_t x, uint16_t y, uint16_t fontsize, const Color& src) {
    TTF_Font* ourFont = ResourceManager::loadFont(Consts::DEFAULT_FONT_FILENAME)->getFont(fontsize);

    // Confirm that it was loaded
    if(ourFont == nullptr) {
        return;
    }

    SDL_Surface* surfaceText = TTF_RenderText_Solid(ourFont,
                                                    text.c_str(),
                                                    {255, 255, 255, 255});

    uint16_t textWidth = std::min(surfaceText->w, _width - x);
    uint16_t textHeight = std::min(surfaceText->h, _height - y);
    uint16_t pitch = surfaceText->pitch;
    uint8_t *pixels = reinterpret_cast<uint8_t *>(surfaceText->pixels);

    // default SDL blend mode:
    // dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
    // dstA = srcA + (dstA * (1-srcA))

    for (uint16_t i = 0; i < textHeight; i++) {
        for (uint16_t j = 0; j < textWidth; j++) {
            if (pixels[i * pitch + j]) {
                size_t offset = (i + y) * _width + (j + x);
                Color dst(_pixelBuffer[offset]);
                Color res((static_cast<int>(src.r()) * src.a() + static_cast<int>(dst.r()) * (255 - src.a())) / 255,
                          (static_cast<int>(src.g()) * src.a() + static_cast<int>(dst.g()) * (255 - src.a())) / 255,
                          (static_cast<int>(src.b()) * src.a() + static_cast<int>(dst.b()) * (255 - src.a())) / 255,
                          src.a() + dst.a() * (255 - src.a())
                );
                _pixelBuffer[offset] = res.rgba();
            }
        }
    }

    SDL_FreeSurface(surfaceText);
}

void Screen::close() {
    _isOpen = false;

    SDL_DestroyTexture(_screenTexture);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();

    _screenTexture = nullptr;
    _renderer = nullptr;
    _window = nullptr;
}

Screen::~Screen() {
    if (_screenTexture) {
        SDL_DestroyTexture(_screenTexture);
        _screenTexture = nullptr;
    }

    if(_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if(_width) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

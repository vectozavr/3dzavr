#include <algorithm>
#include <utility>
#include <cmath>

#include "SDL.h"

#include <io/Screen.h>
#include <utils/Time.h>
#include <utils/Log.h>
#include <utils/stack_vector.h>
#include <utils/ResourceManager.h>
#include <objects/lighting/DirectionalLight.h>

extern "C" {
#include "io/microui/microui.h"
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define popen _popen
#endif


void Screen::open(uint16_t screenWidth, uint16_t screenHeight, const Color& background) {
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

    Log::log("Screen::open(): initialized and opened the screen");
}

void Screen::display() {

    std::string title = _title + " (" + std::to_string(Time::fps()) + " fps)";

    if(_renderVideo && (Time::time() - _lastFrameTime) >= 1.0/_clipFps) {
        // most of the time of video rendering is wasting on saving .png sequence
        // that's why we will save all images in the end
        _lastFrameTime = Time::time();
        png_bytep _data = Image(_pixelBuffer, width(), height()).data();
        fwrite(_data, sizeof(png_byte), _height * _width * 4, _ffmpeg);
    }

    if(_isOpen) {
        SDL_UpdateTexture(_screenTexture, NULL, _pixelBuffer.data(), _width * 4);
        SDL_RenderCopy(_renderer, _screenTexture, NULL, NULL);
        SDL_RenderPresent(_renderer);
    }
}

void Screen::startRender() {
    if(_renderVideo) {
        stopRender();
    }

    _lastFrameTime = Time::time();

    popen("mkdir -p film", "w");

    Log::log("Screen::startRender(): start recording the screen");

    auto cmd = "ffmpeg -f rawvideo -pixel_format rgba -video_size " +
               std::to_string(_width) + "x" + std::to_string(_height) +
               " -framerate " + std::to_string(_clipFps) + " -i - -c:v libx264 -crf 17 -pix_fmt yuv420p -tune animation film/clip_" +
               Time::getLocalTimeInfo("%F_%H-%M-%S") + ".mp4";
    _ffmpeg = popen(cmd.c_str(), "w");

    _renderVideo = true;
}

void Screen::stopRender() {
    if(_renderVideo) {
        Log::log("Screen::stopRender(): stop recording the screen");

        pclose(_ffmpeg);
        _ffmpeg = nullptr;

        _renderVideo = false;
        Log::log("Screen::stopRender(): finish rendering final video");
    }
}

void Screen::clear() {
    std::fill(_depthBuffer.begin(), _depthBuffer.end(), 1.0f);
    std::fill(_pixelBuffer.begin(), _pixelBuffer.end(), _background.rgba());
}

void Screen::drawPixel(int x, const int y, const Color &color) {
    if(x >= _width || y >= _height || x < 0 || y < 0)
        return;

    drawPixelUnsafe(x, y, color);
}

void Screen::drawPixel(int x, int y, double z, const Color &color) {
    if(x >= _width || y >= _height || x < 0 || y < 0)
        return;

    if(checkPixelDepth(x, y, z)) {
        drawPixelUnsafe(x, y, color);
        _depthBuffer[y * _width + x] = z;
    }
}

inline void Screen::drawPixelUnsafe(uint16_t x, uint16_t y, const Color &color) {
    size_t offset = y * _width + x;

    if (color.a() == 255 || !_enableTransparency) {
        _pixelBuffer[offset] = color.rgba();
    } else {
        _pixelBuffer[offset] = color.blend(Color(_pixelBuffer[offset])).rgba();
    }
}

inline void Screen::drawPixelUnsafe(uint16_t x, uint16_t y, double z, const Color &color) {
    if (checkPixelDepth(x, y, z)) {
        drawPixelUnsafe(x, y, color);

        if(color.a() == 255 || !_enableTransparency) {
            _depthBuffer[y * _width + x] = z;
        }
    }
}

inline bool Screen::checkPixelDepth(uint16_t x, uint16_t y, double z) const {
    return z < _depthBuffer[y * _width + x] || !_depthTest;
}

void Screen::plotLineLow(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness) {
    int dx = x_to - x_from;
    int dy = y_to - y_from;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y_from;

    for (int x = x_from; x <= x_to; x++) {
        if(thickness == 1) {
            drawPixel(x, y, color);
        } else {
            drawCircle(x, y, thickness / 2, color);
        }

        if (D > 0) {
            y += yi;
            D += 2 * (dy - dx);
        } else {
            D += 2 * dy;
        }
    }
}

void Screen::plotLineHigh(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness) {
    int dx = x_to - x_from;
    int dy = y_to - y_from;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x_from;

    for (int y = y_from; y <= y_to; y++) {
        if(thickness == 1) {
            drawPixel(x, y, color);
        } else {
            drawCircle(x, y, thickness / 2, color);
        }

        if (D > 0) {
            x += xi;
            D += 2 * (dx - dy);
        } else {
            D += 2 * dx;
        }
    }
}

void Screen::drawLine(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness) {
    if (std::abs(y_to - y_from) < std::abs(x_to - x_from)) {
        if (x_from > x_to) {
            plotLineLow(x_to, y_to, x_from, y_from, color, thickness);
        } else {
            plotLineLow(x_from, y_from, x_to, y_to, color, thickness);
        }
    } else {
        if (y_from > y_to) {
            plotLineHigh(x_to, y_to, x_from, y_from, color, thickness);
        } else {
            plotLineHigh(x_from, y_from, x_to, y_to, color, thickness);
        }
    }
}

void Screen::drawLine(const Vec2D &from, const Vec2D &to, const Color &color, uint16_t thickness) {
    drawLine((int)from.x(), (int)from.y(), (int)to.x(), (int)to.y(), color, thickness);
}

inline bool isInsideTriangleAbg(const Vec3D& abg, double eps = 0) {
    return abg.x() >= -eps && abg.y() >= -eps && abg.z() >= -eps;
}

/*
 * This function returns the x limits within we want to draw:
 * we draw the triangle line by line from up to down, hence for each Y coordinate
 * we compute the limits:
 * x_line_min - start drawing from this coordinate
 * x_line_max - finish drawing at this coordinate.
 */
bool lineLimits(const Vec3D& abg, const Vec3D& abg_dx, uint16_t x_min, uint16_t x_max, uint16_t& x_line_min, uint16_t& x_line_max) {
    stack_vector<double, 4> offsets = stack_vector<double, 4>();
    if (isInsideTriangleAbg(abg)) {
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

    while (offsets.size() >= 2) {
        modifier = (offsets[offsets.size() - 1] + offsets[offsets.size() - 2]) / 2;
        if (isInsideTriangleAbg(abg + abg_dx * modifier)) {
            break;
        } else {
            offsets.pop_back();
        }
    }
    if (offsets.size() < 2) {
        return false;
    }

    x_line_min = std::clamp<uint16_t>(x_min + std::ceil(offsets.back() - Consts::EPS), x_min, x_max);
    offsets.pop_back();
    x_line_max = std::clamp<uint16_t>(x_min + std::floor(offsets.back() + Consts::EPS), x_min, x_max);
    return true;
}

inline double areaDuDv(const Vec3D& uv_hom,
                       const Vec2D& uv_dehom,
                       const Vec3D& uv_hom_dx,
                       const Vec3D& uv_hom_dy,
                       double x, double y,
                       double x_min, double y_min,
                       uint16_t imageWidth, uint16_t imageHeight) {
    // Derivative in X
    Vec3D uv_hom_xp = uv_hom + uv_hom_dx;
    Vec2D uv_dehom_xp = Vec2D(uv_hom_xp.x() / uv_hom_xp.z(), uv_hom_xp.y() / uv_hom_xp.z()) - uv_dehom;

    // Derivative in Y
    Vec3D uv_hom_yp = uv_hom + uv_hom_dy;
    Vec2D uv_dehom_yp = Vec2D(uv_hom_yp.x() / uv_hom_yp.z(), uv_hom_yp.y() / uv_hom_yp.z()) - uv_dehom;

    // Derivative in XY combined [Jacobian]
    Vec2D du(imageWidth * uv_dehom_xp.x(), imageWidth * uv_dehom_yp.x());
    Vec2D dv(imageHeight * uv_dehom_xp.y(), imageHeight * uv_dehom_yp.y());

    return du.abs() + dv.abs();
}

struct Vec3DUint {
    uint r = 0;
    uint g = 0;
    uint b = 0;

    inline Vec3DUint& operator+=(const Vec3DUint& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    };

    [[nodiscard]] inline Vec3DUint operator+(const Vec3DUint& other) const {
        Vec3DUint res = *this;
        res += other;
        return res;
    }

    [[nodiscard]] inline Vec3DUint operator*(double number) const {
        return {static_cast<uint>(r*number),
                static_cast<uint>(g*number),
                static_cast<uint>(b*number)};
    }
};


std::tuple<Vec3DUint, Vec3DUint, Vec3DUint> computeLightingForThreePoints(const Triangle &Mtriangle,
                                                    const std::vector<std::shared_ptr<LightSource>>& lights) {
    Vec3DUint l1, l2, l3;

    for (const auto& lightSource: lights) {
        auto light = std::dynamic_pointer_cast<LightSource>(lightSource);
        auto c1 = light->illuminate(Mtriangle.norm(), Vec3D(Mtriangle[0]));
        auto c2 = light->illuminate(Mtriangle.norm(), Vec3D(Mtriangle[1]));
        auto c3 = light->illuminate(Mtriangle.norm(), Vec3D(Mtriangle[2]));

        l1 += {c1.r(), c1.g(), c1.b()};
        l2 += {c2.r(), c2.g(), c2.b()};
        l3 += {c3.r(), c3.g(), c3.b()};
    }

    return {l1, l2, l3};
}


void Screen::drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                      const std::vector<std::shared_ptr<LightSource>>& lights, Material* material) {

    if(!_enableLighting) {
        drawTriangle(projectedTriangle, material);
        return;
    }

    if (!material || !material->texture() || !_enableTexturing) {
        Color color;
        if (material == nullptr) {
            color = Color::RED;
        } else {
            color = material->ambient();
            color[3] *= material->d();
        }
        drawTriangleWithLighting(projectedTriangle, Mtriangle, lights, color);
        return;
    }

    if(material->illum() != 1) {
        drawTriangle(projectedTriangle, material);
        return;
    }

    // Filling inside
    auto x_min = std::clamp<uint16_t>(std::ceil(std::min({projectedTriangle[0].x(), projectedTriangle[1].x(), projectedTriangle[2].x()})), 0, _width - 1);
    auto y_min = std::clamp<uint16_t>(std::ceil(std::min({projectedTriangle[0].y(), projectedTriangle[1].y(), projectedTriangle[2].y()})), 0, _height - 1);
    auto x_max = std::clamp<uint16_t>(std::floor(std::max({projectedTriangle[0].x(), projectedTriangle[1].x(), projectedTriangle[2].x()})), 0, _width - 1);
    auto y_max = std::clamp<uint16_t>(std::floor(std::max({projectedTriangle[0].y(), projectedTriangle[1].y(), projectedTriangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

    auto& tc = projectedTriangle.textureCoordinates();
    auto texture = material->texture();
    Color color = material->ambient();

    auto abg_origin = projectedTriangle.abgBarycCoord(Vec2D(x_min, y_min));
    /*
     * Here we calculate the change of abg coordinates when we
     * 1) add one pixel in X: abg_dx = abg(triangle[0] + dx) - abg(triangle[0]) = abg(triangle[0] + dx) - {1, 0, 0}
     * 2) add one pixel in Y: abg_dy = abg(triangle[0] + dy) - abg(triangle[0]) = abg(triangle[0] + dy) - {1, 0, 0}
     */
    auto abg_dx = projectedTriangle.abgBarycCoord(Vec2D(projectedTriangle[0]) + Vec2D(1, 0)) - Vec3D(1, 0, 0);
    auto abg_dy = projectedTriangle.abgBarycCoord(Vec2D(projectedTriangle[0]) + Vec2D(0, 1)) - Vec3D(1, 0, 0);

    Vec3D uv_hom_origin, uv_hom_dx, uv_hom_dy;

    uv_hom_origin = tc[0] + (tc[1] - tc[0]) * abg_origin.y() + (tc[2] - tc[0]) * abg_origin.z();
    uv_hom_dx = (tc[1] - tc[0]) * abg_dx.y() + (tc[2] - tc[0]) * abg_dx.z();
    uv_hom_dy = (tc[1] - tc[0]) * abg_dy.y() + (tc[2] - tc[0]) * abg_dy.z();

    // Let us try to do lighting not for every pixel, but for the triangle.
    auto [l1, l2, l3] = computeLightingForThreePoints(Mtriangle, lights);

    for (uint16_t y = y_min; y <= y_max; y++) {
        uint16_t x_cur_min, x_cur_max;
        if (!lineLimits(abg_origin + abg_dy*(y - y_min), abg_dx, x_min, x_max, x_cur_min, x_cur_max)) continue;

        Vec3D abg = abg_origin + abg_dy*(y - y_min) + abg_dx*(x_cur_min - x_min);
        Vec3D uv_hom = uv_hom_origin + uv_hom_dy*(y-y_min) + uv_hom_dx*(x_cur_min - x_min);

        // Instead of exact area we compute average for each horizontal line:
        Vec3D uv_hom_x_avr = uv_hom_origin + uv_hom_dy*(y-y_min) + uv_hom_dx*((x_cur_min+x_cur_max)/2 - x_min);
        Vec2D uv_dehom_x_avr(uv_hom_x_avr.x() / uv_hom_x_avr.z(), uv_hom_x_avr.y() / uv_hom_x_avr.z());

        double area = 0;
        if(_enableMipmapping) {
            area = areaDuDv(uv_hom_x_avr,
                            uv_dehom_x_avr, uv_hom_dx, uv_hom_dy,
                            (x_min+x_max)/2, y, x_min, y_min,
                            texture->width(), texture->height());
        }
        const Image& sample = texture->get_sample(area);

        for (uint16_t x = x_cur_min; x <= x_cur_max; x++) {
            double non_linear_z_hom = projectedTriangle[0].z() * abg.x() + projectedTriangle[1].z() * abg.y() + projectedTriangle[2].z() * abg.z();
            double z_hom = tc[0].z()*abg.x() + tc[1].z()*abg.y() + tc[2].z()*abg.z();

            if(checkPixelDepth(x, y, non_linear_z_hom) && isInsideTriangleAbg(abg, Consts::EPS)) {
                // de-homogenize UV coordinates
                Vec2D uv_dehom(uv_hom.x() / z_hom, uv_hom.y() / z_hom);
                /*
                 * We can calculate the area of Du*Dv for each pixel, but it is computationally inefficient.
                 * Instead, we use averaged area for the horizontal line (calculation is above).
                */
                //double area = areaDuDv(uv_hom, uv_dehom, uv_hom_dx, uv_hom_dy, x, y, x_min, y_min, texture->width(), texture->height());
                //color = texture->get_pixel_from_UV(uv_dehom, area);
                color = sample.get_pixel_from_UV(uv_dehom);
                color[3] *= material->d();


                // Exact calculation of light (non linear and computationally expensive)
                // Here we do homogination and de-homogination part to do the same as we did for textures
                Vec3D dehom_abg(abg.x() * tc[0].z() / z_hom, abg.y() * tc[1].z() / z_hom, abg.z() * tc[2].z() / z_hom);

                Vec3DUint l;
                if(!_enableTrueLighting) {
                    // Linearization of light:
                    l = l1*dehom_abg.x() + l2*dehom_abg.y() + l3*dehom_abg.z();

                    // Constant for the whole triangle
                    //Vec3DUint l = l1;

                } else {
                    auto dehomPixelPosition = Vec4D(
                            Mtriangle[0] * dehom_abg.x() +
                            Mtriangle[1] * dehom_abg.y() +
                            Mtriangle[2] * dehom_abg.z());
                    for (const auto &lightSource: lights) {
                        auto light = std::dynamic_pointer_cast<LightSource>(lightSource);
                        auto cl = light->illuminate(Mtriangle.norm(), Vec3D(dehomPixelPosition));
                        l += {cl.r(), cl.g(), cl.b()};
                    }
                }

                //TODO: we need to handle light aliasing problem (some sort of mip-mapping, but for normals and lighting...)

                Color resColor(std::clamp<int>(color.r()*l.r/255, 0, 255),
                               std::clamp<int>(color.g()*l.g/255, 0, 255),
                               std::clamp<int>(color.b()*l.b/255, 0, 255), color.a());

                drawPixelUnsafe(x, y, non_linear_z_hom, resColor);
            }
            abg += abg_dx;
            uv_hom += uv_hom_dx;
        }
    }

    // Drawing edge
    if(_enableTriangleBorders) {
        drawLine(Vec2D(projectedTriangle[0]), Vec2D(projectedTriangle[1]), Color::BLACK);
        drawLine(Vec2D(projectedTriangle[1]), Vec2D(projectedTriangle[2]), Color::BLACK);
        drawLine(Vec2D(projectedTriangle[2]), Vec2D(projectedTriangle[0]), Color::BLACK);
    }
}

void Screen::drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                      const std::vector<std::shared_ptr<LightSource>> &lights, const Color &color) {

    if(!_enableLighting) {
        drawTriangle(projectedTriangle, color);
        return;
    }

    // Filling inside
    auto x_min = std::clamp<uint16_t>(std::ceil(std::min({projectedTriangle[0].x(), projectedTriangle[1].x(), projectedTriangle[2].x()})), 0, _width - 1);
    auto y_min = std::clamp<uint16_t>(std::ceil(std::min({projectedTriangle[0].y(), projectedTriangle[1].y(), projectedTriangle[2].y()})), 0, _height - 1);
    auto x_max = std::clamp<uint16_t>(std::floor(std::max({projectedTriangle[0].x(), projectedTriangle[1].x(), projectedTriangle[2].x()})), 0, _width - 1);
    auto y_max = std::clamp<uint16_t>(std::floor(std::max({projectedTriangle[0].y(), projectedTriangle[1].y(), projectedTriangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

    auto& tc = projectedTriangle.textureCoordinates();

    auto abg_origin = projectedTriangle.abgBarycCoord(Vec2D(x_min, y_min));
    /*
     * Here we calculate the change of abg coordinates when we
     * 1) add one pixel in X: abg_dx = abg(triangle[0] + dx) - abg(triangle[0]) = abg(triangle[0] + dx) - {1, 0, 0}
     * 2) add one pixel in Y: abg_dy = abg(triangle[0] + dy) - abg(triangle[0]) = abg(triangle[0] + dy) - {1, 0, 0}
     */
    auto abg_dx = projectedTriangle.abgBarycCoord(Vec2D(projectedTriangle[0]) + Vec2D(1, 0)) - Vec3D(1, 0, 0);
    auto abg_dy = projectedTriangle.abgBarycCoord(Vec2D(projectedTriangle[0]) + Vec2D(0, 1)) - Vec3D(1, 0, 0);

    auto [l1, l2, l3] = computeLightingForThreePoints(Mtriangle, lights);

    for (uint16_t y = y_min; y <= y_max; y++) {
        uint16_t x_cur_min, x_cur_max;
        if (!lineLimits(abg_origin + abg_dy*(y - y_min), abg_dx, x_min, x_max, x_cur_min, x_cur_max)) continue;

        Vec3D abg = abg_origin + abg_dy*(y - y_min) + abg_dx*(x_cur_min - x_min);

        for (int x = x_cur_min; x <= x_cur_max; x++) {
            double non_linear_z_hom = projectedTriangle[0].z() * abg.x() + projectedTriangle[1].z() * abg.y() + projectedTriangle[2].z() * abg.z();
            double z_hom = tc[0].z()*abg.x() + tc[1].z()*abg.y() + tc[2].z()*abg.z();

            if (checkPixelDepth(x, y, non_linear_z_hom) && isInsideTriangleAbg(abg, Consts::EPS)) {
                Vec3D dehom_abg(abg.x() * tc[0].z() / z_hom, abg.y() * tc[1].z() / z_hom, abg.z() * tc[2].z() / z_hom);

                Vec3DUint l;
                if(!_enableTrueLighting) {
                    // Linearization of light:
                    l = l1*dehom_abg.x() + l2*dehom_abg.y() + l3*dehom_abg.z();

                    // Constant for the whole triangle
                    //Vec3DUint l = l1;

                } else {
                    auto dehomPixelPosition = Vec4D(
                            Mtriangle[0] * dehom_abg.x() +
                            Mtriangle[1] * dehom_abg.y() +
                            Mtriangle[2] * dehom_abg.z());
                    for (const auto &lightSource: lights) {
                        auto light = std::dynamic_pointer_cast<LightSource>(lightSource);
                        auto cl = light->illuminate(Mtriangle.norm(), Vec3D(dehomPixelPosition));
                        l += {cl.r(), cl.g(), cl.b()};
                    }
                }

                Color resColor(std::clamp<int>(color.r()*l.r/255, 0, 255),
                               std::clamp<int>(color.g()*l.g/255, 0, 255),
                               std::clamp<int>(color.b()*l.b/255, 0, 255), color.a());

                drawPixelUnsafe(x, y, non_linear_z_hom, resColor);
            }

            abg += abg_dx;
        }
    }

    // Drawing edge
    if(_enableTriangleBorders) {
        drawLine(Vec2D(projectedTriangle[0]), Vec2D(projectedTriangle[1]), Color::BLACK);
        drawLine(Vec2D(projectedTriangle[1]), Vec2D(projectedTriangle[2]), Color::BLACK);
        drawLine(Vec2D(projectedTriangle[2]), Vec2D(projectedTriangle[0]), Color::BLACK);
    }
}

void Screen::drawTriangle(const Triangle &triangle, Material *material) {
    if (!material || !material->texture() || !_enableTexturing) {
        Color color;
        if (!material) {
            color = Color::RED;
        } else {
            color = material->ambient();
            color[3] *= material->d();
        }
        drawTriangle(triangle, color);
        return;
    }

    // Filling inside
    auto x_min = std::clamp<uint16_t>(std::ceil(std::min({ triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    auto y_min = std::clamp<uint16_t>(std::ceil(std::min({ triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);
    auto x_max = std::clamp<uint16_t>(std::floor(std::max({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    auto y_max = std::clamp<uint16_t>(std::floor(std::max({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

    auto& tc = triangle.textureCoordinates();
    auto texture = material->texture();
    Color color = material->ambient();

    auto abg_origin = triangle.abgBarycCoord(Vec2D(x_min, y_min));
    /*
     * Here we calculate the change of abg coordinates when we
     * 1) add one pixel in X: abg_dx = abg(triangle[0] + dx) - abg(triangle[0]) = abg(triangle[0] + dx) - {1, 0, 0}
     * 2) add one pixel in Y: abg_dy = abg(triangle[0] + dy) - abg(triangle[0]) = abg(triangle[0] + dy) - {1, 0, 0}
     */
    auto abg_dx = triangle.abgBarycCoord(Vec2D(triangle[0]) + Vec2D(1, 0)) - Vec3D(1, 0, 0);
    auto abg_dy = triangle.abgBarycCoord(Vec2D(triangle[0]) + Vec2D(0, 1)) - Vec3D(1, 0, 0);

    Vec3D uv_hom_origin, uv_hom_dx, uv_hom_dy;

    uv_hom_origin = tc[0] + (tc[1] - tc[0]) * abg_origin.y() + (tc[2] - tc[0]) * abg_origin.z();
    uv_hom_dx = (tc[1] - tc[0]) * abg_dx.y() + (tc[2] - tc[0]) * abg_dx.z();
    uv_hom_dy = (tc[1] - tc[0]) * abg_dy.y() + (tc[2] - tc[0]) * abg_dy.z();

    for (uint16_t y = y_min; y <= y_max; y++) {
        uint16_t x_cur_min, x_cur_max;

        if (!lineLimits(abg_origin + abg_dy*(y - y_min), abg_dx, x_min, x_max, x_cur_min, x_cur_max)) continue;

        Vec3D abg = abg_origin + abg_dy*(y - y_min) + abg_dx*(x_cur_min - x_min);
        Vec3D uv_hom = uv_hom_origin + uv_hom_dy*(y-y_min) + uv_hom_dx*(x_cur_min - x_min);

        // Instead of exact area we compute average for each horizontal line:
        Vec3D uv_hom_x_avr = uv_hom_origin + uv_hom_dy*(y-y_min) + uv_hom_dx*((x_cur_min+x_cur_max)/2 - x_min);
        Vec2D uv_dehom_x_avr(uv_hom_x_avr.x() / uv_hom_x_avr.z(), uv_hom_x_avr.y() / uv_hom_x_avr.z());

        double area = 0;
        if(_enableMipmapping) {
            area = areaDuDv(uv_hom_x_avr, uv_dehom_x_avr, uv_hom_dx, uv_hom_dy, (x_min+x_max)/2, y, x_min, y_min, texture->width(), texture->height());
        }
        const Image& sample = texture->get_sample(area);

        for (uint16_t x = x_cur_min; x <= x_cur_max; x++) {
            double non_linear_z_hom = triangle[0].z() * abg.x() + triangle[1].z() * abg.y() + triangle[2].z() * abg.z();
            double z_hom = tc[0].z()*abg.x() + tc[1].z()*abg.y() + tc[2].z()*abg.z();

            if(checkPixelDepth(x, y, non_linear_z_hom) && isInsideTriangleAbg(abg, Consts::EPS)) {
                // de-homogenize UV coordinates
                Vec2D uv_dehom(uv_hom.x() / z_hom, uv_hom.y() / z_hom);
                /*
                 * We can calculate the area of Du*Dv for each pixel, but it is computationally inefficient.
                 * Instead, we use averaged area for the horizontal line (calculation is above).
                */
                //double area = areaDuDv(uv_hom, uv_dehom, uv_hom_dx, uv_hom_dy, x, y, x_min, y_min, texture->width(), texture->height());
                //color = texture->get_pixel_from_UV(uv_dehom, area);
                color = sample.get_pixel_from_UV(uv_dehom);
                color[3] *= material->d();
                drawPixelUnsafe(x, y, non_linear_z_hom, color);
            }
            abg += abg_dx;
            uv_hom += uv_hom_dx;
        }
    }

    // Drawing edge
    if(_enableTriangleBorders) {
        drawLine(Vec2D(triangle[0]), Vec2D(triangle[1]), Color::BLACK);
        drawLine(Vec2D(triangle[1]), Vec2D(triangle[2]), Color::BLACK);
        drawLine(Vec2D(triangle[2]), Vec2D(triangle[0]), Color::BLACK);
    }
}

void Screen::drawTriangle(const Triangle &triangle, const Color &color) {
    // Filling inside
    auto x_min = std::clamp<uint16_t>(std::ceil(std::min({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    auto y_min = std::clamp<uint16_t>(std::ceil(std::min({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);
    auto x_max = std::clamp<uint16_t>(std::floor(std::max({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    auto y_max = std::clamp<uint16_t>(std::floor(std::max({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

    auto abg_origin = triangle.abgBarycCoord(Vec2D(x_min, y_min));
    /*
     * Here we calculate the change of abg coordinates when we
     * 1) add one pixel in X: abg_dx = abg(triangle[0] + dx) - abg(triangle[0]) = abg(triangle[0] + dx) - {1, 0, 0}
     * 2) add one pixel in Y: abg_dy = abg(triangle[0] + dy) - abg(triangle[0]) = abg(triangle[0] + dy) - {1, 0, 0}
     */
    auto abg_dx = triangle.abgBarycCoord(Vec2D(triangle[0]) + Vec2D(1, 0)) - Vec3D(1, 0, 0);
    auto abg_dy = triangle.abgBarycCoord(Vec2D(triangle[0]) + Vec2D(0, 1)) - Vec3D(1, 0, 0);

    for (uint16_t y = y_min; y <= y_max; y++) {
        uint16_t x_cur_min, x_cur_max;
        if (!lineLimits(abg_origin + abg_dy*(y - y_min), abg_dx, x_min, x_max, x_cur_min, x_cur_max)) continue;

        Vec3D abg = abg_origin + abg_dy*(y - y_min) + abg_dx*(x_cur_min - x_min);

        for (int x = x_cur_min; x <= x_cur_max; x++) {
            if(isInsideTriangleAbg(abg, Consts::EPS)) {
                double non_linear_z_hom = triangle[0].z() * abg.x() + triangle[1].z() * abg.y() + triangle[2].z() * abg.z();
                drawPixelUnsafe(x, y, non_linear_z_hom, color);
            }

            abg += abg_dx;
        }
    }

    // Drawing edge
    if(_enableTriangleBorders) {
        drawLine(Vec2D(triangle[0]), Vec2D(triangle[1]), Color::BLACK);
        drawLine(Vec2D(triangle[1]), Vec2D(triangle[2]), Color::BLACK);
        drawLine(Vec2D(triangle[2]), Vec2D(triangle[0]), Color::BLACK);
    }
}

void Screen::setTitle(const std::string &title) {
    _title = title;
    SDL_SetWindowTitle(_window, title.c_str());
}

bool Screen::isOpen() const {
    return _isOpen;
}

void Screen::drawRectangle(int x, int y, uint16_t width, uint16_t height, const Color &color) {
    for(int _y = y; _y < y + height; _y++) {
        for(int _x = x; _x < x + width; _x++) {
            drawPixel(_x, _y, color);
        }
    }
}

void Screen::drawStrokeRectangle(int x, int y, uint16_t width, uint16_t height, const Color &color,
                                 uint16_t thickness, const Color &strokeColor) {
    drawRectangle(x, y, width, height, color);
    drawLine(x, y, x + width, y, strokeColor);
    drawLine(x+ width, y, x + width, y + height, strokeColor);
    drawLine(x + width, y + height, x, y + height, strokeColor);
    drawLine(x, y + height, x, y, strokeColor);
}

void Screen::drawImage(int x, int y, std::shared_ptr<Image> img) {
    for(int i = 0; i < img->width(); i++) {
        for(int j = 0; j < img->height(); j++) {
            drawPixel(i + x, j + y, img->get_pixel(i, j));
        }
    }
}

void Screen::drawText(const std::string& text, int x, int y, const Color& color, uint16_t fontsize, const std::shared_ptr<Font>& font) {
    TTF_Font* currentFont;
    if(!font) {
        currentFont = ResourceManager::loadFont(Consts::DEFAULT_FONT_FILENAME)->getFont(fontsize);
    } else {
        currentFont = font->getFont(fontsize);
    }

    if(!currentFont) {
        return;
    }

    SDL_Surface* surfaceText = TTF_RenderText_Solid(currentFont,
                                                    text.c_str(),
                                                    {255, 255, 255, 255});

    if(!surfaceText) {
        return;
    }

    uint16_t textWidth = std::min(surfaceText->w, _width - x);
    uint16_t textHeight = std::min(surfaceText->h, _height - y);
    uint16_t pitch = surfaceText->pitch;
    uint8_t *pixels = reinterpret_cast<uint8_t *>(surfaceText->pixels);

    for (uint16_t i = 0; i < textHeight; i++) {
        for (uint16_t j = 0; j < textWidth; j++) {
            if (pixels[i * pitch + j]) {
                if(j + x < _width && j + x > 0 && i + y < _height && i + y > 0)
                    drawPixelUnsafe(j + x, i + y, color);
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

void Screen::drawPlot(const std::vector<std::pair<double, double>> &data, int x, int y, uint16_t w, uint16_t h) {
    double minX = std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();

    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& [xVal, yVal] : data) {
        if (xVal < minX) minX = xVal;
        if (yVal < minY) minY = yVal;
        if (xVal > maxX) maxX = xVal;
        if (yVal > maxY) maxY = yVal;
    }

    if(std::abs(maxX-minX) < Consts::EPS) {
        maxX += minX/2 + Consts::EPS;
    }
    if(std::abs(maxY-minY) < Consts::EPS) {
        maxY += minY/2 + Consts::EPS;
    }

    drawStrokeRectangle(x, y, w, h, Color(255, 255, 255, 50), 1);

    auto [xPrev, yPrev] = data.front();
    for (const auto& [xVal, yVal] : data) {
        drawLine(x + (xPrev-minX)*w/(maxX-minX), y + h - (yPrev-minY)*h/(maxY-minY),
                 x + (xVal-minX)*w/(maxX-minX), y + h - (yVal-minY)*h/(maxY-minY), Color::RED);

        xPrev = xVal;
        yPrev = yVal;
    }
}

Image Screen::makeScreenShot() {
    return {_pixelBuffer, width(), height()};
}

void Screen::drawRectangle(int x, int y, uint16_t width, uint16_t height, Material *material) {
    Triangle tri1({Vec4D(x, y), Vec4D(x+width, y), Vec4D(x, y+height)},
             {Vec3D(0, 0, 1), Vec3D(1, 0, 1), Vec3D(0, 1, 1)});
    Triangle tri2({Vec4D(x, y+height), Vec4D(x+width, y), Vec4D(x+width, y+height)},
             {Vec3D(0, 1, 1), Vec3D(1, 0, 1), Vec3D(1, 1, 1)});

    drawTriangle(tri1, material);
    drawTriangle(tri2, material);
}

void Screen::drawCircle(int x, int y, uint16_t r, const Color &fillColor) {
    for(int _y = y-r; _y <= y+r; _y++) {
        for(int _x = x-r; _x <= x+r; _x++) {
            double d2 = (_x-x)*(_x-x) + (_y-y)*(_y-y);
            if(d2 <= r*r) {
                drawPixel(_x, _y, fillColor);
            }
        }
    }
}

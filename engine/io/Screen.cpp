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
    std::fill(_pixelBuffer.begin(), _pixelBuffer.end(), _background.rgba());
}

void Screen::drawPixel(const uint16_t x, const uint16_t y, const Color &color) {
    if(x >= _width || x < 0 || y >= _height || y < 0)
        return;

    drawPixelUnsafe(x, y, color);
}

void Screen::drawPixel(uint16_t x, uint16_t y, double z, const Color &color) {
    if(x >= _width || x < 0 || y >= _height || y < 0)
        return;

    if(checkPixelDepth(x, y, z)) {
        drawPixelUnsafe(x, y, color);
        _depthBuffer[y * _width + x] = z;
    }
}

inline void Screen::drawPixelUnsafe(const uint16_t x, const uint16_t y, const Color &color) {
    if (color.a() == 255) {
        _pixelBuffer[y * _width + x] = color.rgba();
    } else {
        size_t offset = y * _width + x;
        _pixelBuffer[offset] = color.blend(Color(_pixelBuffer[offset])).rgba();
    }
}

inline void Screen::drawPixelUnsafe(uint16_t x, uint16_t y, double z, const Color &color) {
    if (checkPixelDepth(x, y, z)) {
        drawPixelUnsafe(x, y, color);

        if(color.a() == 255) {
            _depthBuffer[y * _width + x] = z;
        }
    }
}

inline bool Screen::checkPixelDepth(uint16_t x, uint16_t y, double z) const {
    return z < _depthBuffer[y * _width + x];
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

inline bool isInsideTriangleAbg(const Vec3D& abg) {
    return abg.x() >= 0 && abg.y() >= 0 && abg.z() >= 0;
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
    // Drawing edge
    //drawLine(Vec2D(projectedTriangle[0]), Vec2D(projectedTriangle[1]), Color::BLACK);
    //drawLine(Vec2D(projectedTriangle[1]), Vec2D(projectedTriangle[2]), Color::BLACK);
    //drawLine(Vec2D(projectedTriangle[2]), Vec2D(projectedTriangle[0]), Color::BLACK);

    if (material == nullptr || material->texture() == nullptr) {
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
        double area = areaDuDv(uv_hom_x_avr, uv_dehom_x_avr, uv_hom_dx, uv_hom_dy, (x_min+x_max)/2, y, x_min, y_min, texture->width(), texture->height());
        const Image& sample = texture->get_sample(area);

        for (uint16_t x = x_cur_min; x <= x_cur_max; x++) {
            double z = projectedTriangle[0].z() * abg.x() + projectedTriangle[1].z() * abg.y() + projectedTriangle[2].z() * abg.z();
            if(checkPixelDepth(x, y, z)) {
                // de-homogenize UV coordinates
                Vec2D uv_dehom(uv_hom.x() / uv_hom.z(), uv_hom.y() / uv_hom.z());
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
                /*
                Vec3DUint l;
                auto dehomPixelPosition = Vec4D(
                        Mtriangle[0] * abg.x() * tc[0].z() +
                        Mtriangle[1] * abg.y() * tc[1].z() +
                        Mtriangle[2] * abg.z() * tc[2].z())/uv_hom.z();
                for (const auto& lightSource: lights) {
                    auto light = std::dynamic_pointer_cast<LightSource>(lightSource);
                    auto cl = light->illuminate(Mtriangle.norm(), Vec3D(dehomPixelPosition));
                    l += {cl.r(), cl.g(), cl.b()};
                }
                 */

                // Linearization of light:
                // Here we do homogination and de-homogination part to do the same as we did for textures
                Vec3DUint l =   l1 * std::clamp(abg.x() * tc[0].z() / uv_hom.z(), 0.0, 1.0) +
                                l2 * std::clamp(abg.y() * tc[1].z() / uv_hom.z(), 0.0, 1.0) +
                                l3 * std::clamp(abg.z() * tc[2].z() / uv_hom.z(), 0.0, 1.0);

                // Constant for the whole triangle
                //Vec3DUint l = l1;

                //TODO: we need to handle light aliasing problem (some sort of mini-mapping, but for normals and lighting...)

                Color resColor(std::clamp<int>(color.r()*l.r/255, 0, 255),
                               std::clamp<int>(color.g()*l.g/255, 0, 255),
                               std::clamp<int>(color.b()*l.b/255, 0, 255), color.a());

                drawPixelUnsafe(x, y, z, resColor);
            }
            abg += abg_dx;
            uv_hom += uv_hom_dx;
        }
    }
}

void Screen::drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                      const std::vector<std::shared_ptr<LightSource>> &lights, const Color &color) {
    // Filling inside
    auto x_min = std::clamp<uint16_t>(std::ceil(std::min({projectedTriangle[0].x(), projectedTriangle[1].x(), projectedTriangle[2].x()})), 0, _width - 1);
    auto y_min = std::clamp<uint16_t>(std::ceil(std::min({projectedTriangle[0].y(), projectedTriangle[1].y(), projectedTriangle[2].y()})), 0, _height - 1);
    auto x_max = std::clamp<uint16_t>(std::floor(std::max({projectedTriangle[0].x(), projectedTriangle[1].x(), projectedTriangle[2].x()})), 0, _width - 1);
    auto y_max = std::clamp<uint16_t>(std::floor(std::max({projectedTriangle[0].y(), projectedTriangle[1].y(), projectedTriangle[2].y()})), 0, _height - 1);

    if (x_min > x_max || y_min > y_max) return;

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
            double z = projectedTriangle[0].z() * abg.x() + projectedTriangle[1].z() * abg.y() + projectedTriangle[2].z() * abg.z();

            // Linearization of light:
            Vec3DUint l =   l1 * std::clamp(abg.x(), 0.0, 1.0) +
                            l2 * std::clamp(abg.y(), 0.0, 1.0) +
                            l3 * std::clamp(abg.z(), 0.0, 1.0);

            // Constant for the whole triangle
            //Vec3DUint l = l1;

            Color resColor(std::clamp<int>(color.r()*l.r/255, 0, 255),
                           std::clamp<int>(color.g()*l.g/255, 0, 255),
                           std::clamp<int>(color.b()*l.b/255, 0, 255), color.a());

            drawPixelUnsafe(x, y, z, resColor);
            abg += abg_dx;
        }
    }
}

void Screen::drawTriangle(const Triangle &triangle, Material *material) {
    // Drawing edge
    //drawLine(Vec2D(triangle[0]), Vec2D(triangle[1]), Consts::BLACK);
    //drawLine(Vec2D(triangle[1]), Vec2D(triangle[2]), Consts::BLACK);
    //drawLine(Vec2D(triangle[2]), Vec2D(triangle[0]), Consts::BLACK);

    if (material == nullptr || material->texture() == nullptr) {
        Color color;
        if (material == nullptr) {
            color = Color::RED;
        } else {
            color = material->ambient();
            color[3] *= material->d();
        }
        drawTriangle(triangle, color);
        return;
    }

    // Filling inside
    auto x_min = std::clamp<uint16_t>(std::ceil(std::min({triangle[0].x(), triangle[1].x(), triangle[2].x()})), 0, _width - 1);
    auto y_min = std::clamp<uint16_t>(std::ceil(std::min({triangle[0].y(), triangle[1].y(), triangle[2].y()})), 0, _height - 1);
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
        double area = areaDuDv(uv_hom_x_avr, uv_dehom_x_avr, uv_hom_dx, uv_hom_dy, (x_min+x_max)/2, y, x_min, y_min, texture->width(), texture->height());
        const Image& sample = texture->get_sample(area);

        for (uint16_t x = x_cur_min; x <= x_cur_max; x++) {
            double z = triangle[0].z() * abg.x() + triangle[1].z() * abg.y() + triangle[2].z() * abg.z();
            if(checkPixelDepth(x, y, z)) {
                // de-homogenize UV coordinates
                Vec2D uv_dehom(uv_hom.x() / uv_hom.z(), uv_hom.y() / uv_hom.z());
                /*
                 * We can calculate the area of Du*Dv for each pixel, but it is computationally inefficient.
                 * Instead, we use averaged area for the horizontal line (calculation is above).
                */
                //double area = areaDuDv(uv_hom, uv_dehom, uv_hom_dx, uv_hom_dy, x, y, x_min, y_min, texture->width(), texture->height());
                //color = texture->get_pixel_from_UV(uv_dehom, area);
                color = sample.get_pixel_from_UV(uv_dehom);
                color[3] *= material->d();
                drawPixelUnsafe(x, y, z, color);
            }
            abg += abg_dx;
            uv_hom += uv_hom_dx;
        }
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

    for (uint16_t i = 0; i < textHeight; i++) {
        for (uint16_t j = 0; j < textWidth; j++) {
            if (pixels[i * pitch + j]) {
                size_t offset = (i + y) * _width + (j + x);
                _pixelBuffer[offset] = src.blend(Color(_pixelBuffer[offset])).rgba();
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

void Screen::drawPlot(const std::vector<std::pair<double, double>> &data, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
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
        drawLine(Vec2D(x + (xPrev-minX)*w/(maxX-minX), y + h - (yPrev-minY)*h/(maxY-minY)),
                 Vec2D(x + (xVal-minX)*w/(maxX-minX), y + h - (yVal-minY)*h/(maxY-minY)), Color::RED);

        xPrev = xVal;
        yPrev = yVal;
    }
}

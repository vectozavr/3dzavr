
//
// Created by Ivan Ilin on 21/09/2023.
//

#ifndef RAYTRACE3D_IMAGE_H
#define RAYTRACE3D_IMAGE_H


#include <cstdint>
#include <png.h>

#include <linalg/Vec3D.h>
#include <Consts.h>
#include <utils/FilePath.h>

class Image {
public:
    enum CODE {SUCCESS, FILE_OPEN_ERROR, PNG_STRUCT_ERROR, INFO_STRUCT_ERROR, ERROR};
    enum CLAMP_MODE {REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE};
private:
    uint16_t _width;
    uint16_t _height;

    png_bytep _data = nullptr;

    bool _valid = false;
public:
    explicit Image(uint16_t width = Consts::STANDARD_SCREEN_WIDTH, uint16_t height = Consts::STANDARD_SCREEN_HEIGHT);
    explicit Image(const FilePath &filename);

    Image(const Image& img) = delete;
    Image& operator=(const Image &img) = delete;

    Image(Image&& img) noexcept;
    Image& operator=(Image&& img) noexcept;

    [[nodiscard]] uint16_t width() const { return _width; };
    [[nodiscard]] uint16_t height() const { return _height; };
    [[nodiscard]] bool isValid() const { return _valid; };

    void set_pixel(uint16_t x, uint16_t y, const Color& color);
    [[nodiscard]] Color get_pixel_unsafe(uint16_t x, uint16_t y) const;
    [[nodiscard]] Color get_pixel(uint16_t x, uint16_t y) const;
    [[nodiscard]] Color get_pixel_from_UV(const Vec2D& uv, CLAMP_MODE mode = REPEAT, bool bottomUp = true) const;
    [[nodiscard]] Image downSampled() const;

    CODE save2png(const FilePath& file_name, uint16_t bit_depth = 8);

    ~Image();
};


#endif //RAYTRACE3D_IMAGE_H

#ifndef PROPS_TEXTURE_H
#define PROPS_TEXTURE_H

#include <vector>

#include "io/Image.h"
#include "utils/FilePath.h"

class Texture {
private:
    // For resampling purposes we store resampled versions of the image
    // up until 1x1 image (avg color of the whole texture)
    std::vector<Image> _texture;

    bool _isTransparent = false;

    void checkTransparency();
    void downSample();
public:
    explicit Texture(const FilePath& filename);
    explicit Texture(Image& image);

    [[nodiscard]] Color get_pixel(uint16_t x, uint16_t y) const;
    [[nodiscard]] Color get_pixel_from_UV(const Vec2D& uv) const;

    // For resampling
    [[nodiscard]] const Image& get_sample(double area) const;
    [[nodiscard]] Color get_pixel_from_UV(const Vec2D& uv, double area) const;

    [[nodiscard]] uint16_t width() const { return _texture.front().width(); }
    [[nodiscard]] uint16_t height() const { return _texture.front().height(); }

    [[nodiscard]] bool isTransparent() const {return _isTransparent; }
};


#endif //PROPS_TEXTURE_H

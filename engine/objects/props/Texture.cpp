//
// Created by Ivan Ilin on 04/11/2023.
//

#include <cmath>

#include "Texture.h"
#include <utils/math.h>

Texture::Texture(const FilePath &filename) {
    _texture.emplace_back(filename);
    while (_texture.back().width() * _texture.back().height() != 1) {
        _texture.emplace_back(_texture.back().downSampled());
    }
}

Color Texture::get_pixel(uint16_t x, uint16_t y) const {
    return _texture.front().get_pixel(x, y);
}

Color Texture::get_pixel_from_UV(const Vec2D &uv) const {
    return _texture.front().get_pixel_from_UV(uv);
}

Color Texture::get_pixel_from_UV(const Vec2D &uv, double area) const {
    uint64_t limit = 1ULL << (_texture.size() - 1);
    uint16_t K;

    if (area < 2) {
        K = 0;
    } else if (area < limit) {
        K = static_cast<uint16_t>(log2_u64(area)) - 1;
    } else {
        K = _texture.size() - 1;
    }

    return _texture[K].get_pixel_from_UV(uv);
}

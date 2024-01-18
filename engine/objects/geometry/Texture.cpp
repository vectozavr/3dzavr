//
// Created by Ivan Ilin on 04/11/2023.
//

#include <cmath>
#include <iostream>

#include "Texture.h"

Texture::Texture(const std::string &filename) {
    auto texture = std::make_shared<Image>(filename);
    _texture.emplace_back(texture);

    while (texture->width()*texture->height() != 1) {
        texture = texture->downSampled();

        _texture.emplace_back(texture);
    }
}

Color Texture::get_pixel(uint16_t x, uint16_t y) const {
    return _texture.front()->get_pixel(x, y);
}

Color Texture::get_pixel_from_UV(const Vec2D &uv) const {
    return _texture.front()->get_pixel_from_UV(uv);
}

Color Texture::get_pixel_from_UV(const Vec2D &uv, double area) const {

    auto D = std::log2(area);

    uint16_t K;

    if(D <= 1) {
        K = 0;
    } else if(D > (double)_texture.size() - 1.0) {
        K = _texture.size() - 1;
    } else {
        K  = (uint16_t)D - 1;
    }

    return _texture[K]->get_pixel_from_UV(uv);
}

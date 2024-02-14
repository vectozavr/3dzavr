//
// Created by Ivan Ilin on 13/09/2023.
//

#ifndef RAYTRACE3D_COLOR_H
#define RAYTRACE3D_COLOR_H

#include <array>
#include <cstdint>

class Color final {
private:
    std::array<uint8_t, 4> _c;

public:
    Color(const Color &other) {
        std::copy(other._c.begin(), other._c.end(), _c.begin());
    }

    explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) : _c({r, g, b, a}) {}

    explicit Color(const std::array<uint8_t, 4> &color) : _c(color) {}

    Color &operator=(const Color &color) = default;

    [[nodiscard]] inline uint8_t r() const { return _c[0]; }
    [[nodiscard]] inline uint8_t g() const { return _c[1]; }
    [[nodiscard]] inline uint8_t b() const { return _c[2]; }
    [[nodiscard]] inline uint8_t a() const { return _c[3]; }

    // Operations with colors
    [[nodiscard]] inline bool operator==(const Color &other) const { return _c == other._c; }
    [[nodiscard]] inline bool operator!=(const Color &other) const { return !(*this == other); }
};


#endif //RAYTRACE3D_COLOR_H

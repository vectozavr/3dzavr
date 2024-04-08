#ifndef PROPS_COLOR_H
#define PROPS_COLOR_H

#include <array>
#include <cstdint>

class Color final {
private:
    std::array<uint8_t, 4> _c;

public:
    Color(const Color &other) {
        std::copy(other._c.begin(), other._c.end(), _c.begin());
    }

    explicit Color(uint32_t c) : _c({
        static_cast<unsigned char>((c >> 24) & 0xFF),
        static_cast<unsigned char>((c >> 16) & 0xFF),
        static_cast<unsigned char>((c >> 8) & 0xFF),
        static_cast<unsigned char>(c & 0xFF)
    }) {}

    explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) : _c({r, g, b, a}) {}
    explicit Color(const std::array<uint8_t, 4> &color) : _c(color) {}

    Color &operator=(const Color &color) = default;

    [[nodiscard]] inline uint8_t r() const { return _c[0]; }
    [[nodiscard]] inline uint8_t g() const { return _c[1]; }
    [[nodiscard]] inline uint8_t b() const { return _c[2]; }
    [[nodiscard]] inline uint8_t a() const { return _c[3]; }

    [[nodiscard]] inline uint8_t& operator[](std::size_t i) { return _c[i]; }
    [[nodiscard]] inline const uint8_t& operator[](std::size_t i) const { return _c[i]; }

    [[nodiscard]] inline uint32_t rgba() const { return (_c[0] << 24) | (_c[1] << 16) | (_c[2] << 8) | _c[3]; }

    // Operations with colors
    [[nodiscard]] inline bool operator==(const Color &other) const { return _c == other._c; }
    [[nodiscard]] inline bool operator!=(const Color &other) const { return !(*this == other); }

    [[nodiscard]] inline Color operator*(double alpha) const { return Color((uint8_t)(_c[0]*alpha),
                                                                            (uint8_t)(_c[1]*alpha),
                                                                            (uint8_t)(_c[2]*alpha),
                                                                            (uint8_t)(_c[3]*alpha)); };
    [[nodiscard]] inline Color operator+(const Color &other) const { return Color(r()+other.r(),
                                                                                  g()+other.g(),
                                                                                  b()+other.b(),
                                                                                  a()+other.a()); };

    [[nodiscard]] inline Color blend(const Color& other) const {
        // default SDL blend mode:
        // dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
        // dstA = srcA + (dstA * (1-srcA))
        uint8_t ra = 255 - a();
        return Color(
            (static_cast<uint16_t>(r()) * a() + other.r() * ra) / 255,
            (static_cast<uint16_t>(g()) * a() + other.g() * ra) / 255,
            (static_cast<uint16_t>(b()) * a() + other.b() * ra) / 255,
            (static_cast<uint16_t>(a()) * 255 + other.a() * ra) / 255
        );
    }

    static const Color WHITE;
    static const Color BLACK;
    static const Color GRAY;
    static const Color WHITE_GRAY;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color LIGHT_YELLOW;
};

#endif //PROPS_COLOR_H

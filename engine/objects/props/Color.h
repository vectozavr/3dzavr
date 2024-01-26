//
// Created by Ivan Ilin on 13/09/2023.
//

#ifndef RAYTRACE3D_COLOR_H
#define RAYTRACE3D_COLOR_H

#include <array>

class Color final {
private:
    std::array<double, 4> _c{};

public:
    Color(const Color &color);

    explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);
    explicit Color(const std::array<double, 4>& color);

    Color &operator=(const Color &color) = default;

    [[nodiscard]] uint8_t r() const;
    [[nodiscard]] uint8_t g() const;
    [[nodiscard]] uint8_t b() const;
    [[nodiscard]] uint8_t a() const;

    // Operations with colors
    [[nodiscard]] Color operator+(const Color &color) const;
    [[nodiscard]] Color operator-(const Color &color) const;
    [[nodiscard]] bool operator==(const Color &color) const;
    [[nodiscard]] Color operator*(const Color &color) const;

    // Operations with numbers
    [[nodiscard]] Color operator*(double number) const;
    [[nodiscard]] Color operator/(double number) const;

    // Other useful methods
    [[nodiscard]] Color normalized() const; // Returns normalized vector without changing
};


#endif //RAYTRACE3D_COLOR_H

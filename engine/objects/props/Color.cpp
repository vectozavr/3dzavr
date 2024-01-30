//
// Created by Ivan Ilin on 13/09/2023.
//


#include <cmath>
#include <stdexcept>

#include "Color.h"
#include "Consts.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _c[0] = (float)r/255.0f;
    _c[1] = (float)g/255.0f;
    _c[2] = (float)b/255.0f;
    _c[3] = (float)a/255.0f;
}

Color::Color(const std::array<double, 4> &color) : _c(color) {

}

Color::Color(const Color &color) {
    _c[0] = color._c[0];
    _c[1] = color._c[1];
    _c[2] = color._c[2];
    _c[3] = color._c[3];
}

uint8_t Color::r() const {
    return (uint8_t)std::min(std::max(_c[0], 0.0)*255.0, 255.0);
}

uint8_t Color::g() const {
    return (uint8_t)std::min(std::max(_c[1], 0.0)*255.0, 255.0);
}

uint8_t Color::b() const {
    return (uint8_t)std::min(std::max(_c[2], 0.0)*255.0, 255.0);
}

uint8_t Color::a() const {
    return (uint8_t)std::min(std::max(_c[3], 0.0)*255.0, 255.0);
}


// Operations with Vec4D
Color Color::operator+(const Color &color) const {
    return Color(std::array<double, 4>{
        _c[0] + color._c[0],
        _c[1] + color._c[1],
        _c[2] + color._c[2],
        _c[3] + color._c[3]});
}

Color Color::operator-(const Color &color) const {
    return Color(std::array<double, 4>{
        _c[0] - color._c[0],
        _c[1] - color._c[1],
        _c[2] - color._c[2],
        _c[3] - color._c[3]});
}

Color Color::operator*(double number) const {
    return Color(std::array<double, 4>{
        _c[0]*number,
        _c[1]*number,
        _c[2]*number,
        _c[3]});
}

Color Color::operator/(double number) const {
    if (std::abs(number) > Consts::EPS) {
        return *this * (1.0f / number);
    } else {
        throw std::domain_error{"Color::operator/(double number): division by zero"};
    }
}

Color Color::normalized() const {
    double vecAbs = sqrt(r()*r() + g()*g() + b()*b() + a()*a());
    if (vecAbs > Consts::EPS) {
        return *this / vecAbs;
    } else {
        return Color{};
    }
}

bool Color::operator==(const Color &color) const {
    return Vec4D(r(), g(), b(), a()) == Vec4D(color.r(), color.g(), color.b(), color.a());
}

Color Color::operator*(const Color &color) const {
    return Color(std::array<double, 4>{
        _c[0]*color._c[0],
        _c[1]*color._c[1],
        _c[2]*color._c[2],
        _c[3]*color._c[3]});
}

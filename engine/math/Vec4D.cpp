//
// Created by Иван Ильин on 12.01.2021.
//

#include <cmath>
#include <stdexcept>

#include "Vec4D.h"
#include "../Consts.h"

Vec4D::Vec4D(double x, double y, double z, double w) {
    _arr_point[0] = x;
    _arr_point[1] = y;
    _arr_point[2] = z;
    _arr_point[3] = w;
}

Vec4D::Vec4D(const Vec4D &point4D) {
    _arr_point[0] = point4D.x();
    _arr_point[1] = point4D.y();
    _arr_point[2] = point4D.z();
    _arr_point[3] = point4D.w();
}

[[nodiscard]] Vec4D Vec4D::operator-() const {
    return Vec4D(-x(), -y(), -z(), -w());
}

bool Vec4D::operator==(const Vec4D &point4D) const {
    return (*this - point4D).sqrAbs() < Consts::EPS;
}

bool Vec4D::operator!=(const Vec4D &point4D) const {
    return !(*this == point4D);
}

// Operations with Vec4D
Vec4D Vec4D::operator+(const Vec4D &point4D) const {
    return Vec4D(x() + point4D.x(), y() + point4D.y(), z() + point4D.z(), w() + point4D.w());
}

Vec4D Vec4D::operator-(const Vec4D &point4D) const {
    return *this + -point4D;
}

Vec4D Vec4D::operator*(double number) const {
    return Vec4D(x() * number, y() * number, z() * number, w() * number);
}

Vec4D Vec4D::operator/(double number) const {
    if (std::abs(number) > Consts::EPS) {
        return *this * (1.0 / number);
    } else {
        throw std::domain_error{"Vec4D::operator/(double number): division by zero"};
    }
}

// Other useful methods
double Vec4D::sqrAbs() const {
    return x() * x() + y() * y() + z() * z() + w() * w();
}

double Vec4D::abs() const {
    return sqrt(sqrAbs());
}

Vec4D Vec4D::normalized() const {
    double vecAbs = sqrAbs();
    if (vecAbs > Consts::EPS) {
        return *this / sqrt(vecAbs);
    } else {
        return Vec4D(1);
    }
}

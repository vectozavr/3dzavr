//
// Created by Иван Ильин on 10.10.2021.
//

#include <cmath>

#include "Vec2D.h"
#include "../Consts.h"

Vec2D::Vec2D(const Vec2D &vec) {
    _arr_point[0] = vec.x();
    _arr_point[1] = vec.y();
}

Vec2D::Vec2D(double x, double y) {
    _arr_point[0] = x;
    _arr_point[1] = y;
}

Vec2D::Vec2D(const Vec4D &point4D) {
    _arr_point[0] = point4D.x();
    _arr_point[1] = point4D.y();
}

Vec2D Vec2D::operator-() const {
    return Vec2D(-x(), -y());
}

bool Vec2D::operator==(const Vec2D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

bool Vec2D::operator!=(const Vec2D &vec) const {
    return !(*this == vec);
}

Vec2D Vec2D::operator+(const Vec2D &vec) const {
    return Vec2D(x() + vec.x(), y() + vec.y());
}

Vec2D Vec2D::operator-(const Vec2D &vec) const {
    return *this + -vec;
}

Vec2D Vec2D::operator*(double number) const {
    return Vec2D(x() * number, y() * number);
}

Vec2D Vec2D::operator/(double number) const {
    if (std::abs(number) > Consts::EPS) {
        return *this * (1.0 / number);
    } else {
        throw std::domain_error{"Vec2D::operator/(double number): division by zero"};
    }
}

// Other useful methods
double Vec2D::sqrAbs() const {
    return x() * x() + y() * y();
}

double Vec2D::abs() const {
    return sqrt(sqrAbs());
}

Vec2D Vec2D::normalized() const {
    double vecAbs = sqrAbs();
    if (vecAbs > Consts::EPS) {
        return *this / sqrt(vecAbs);
    } else {
        return Vec2D(0);
    }
}

double Vec2D::dot(const Vec2D &vec) const {
    return vec.x() * x() + vec.y() * y();
}

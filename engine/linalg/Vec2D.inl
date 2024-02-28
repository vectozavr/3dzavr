//
// Created by Иван Ильин on 10.10.2021.
//

#include <cmath>
#include <stdexcept>

#include <ScalarConsts.h>

inline Vec2D::Vec2D(const Vec2D &vec) : _arr_point{vec[0], vec[1]} {}

inline Vec2D::Vec2D(const Vec4D &vec) : _arr_point{vec[0], vec[1]} {}

inline Vec2D::Vec2D(double x, double y) : _arr_point{x, y} {}

inline Vec2D Vec2D::operator-() const {
    return Vec2D(-x(), -y());
}


inline bool Vec2D::operator==(const Vec2D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

inline bool Vec2D::operator!=(const Vec2D &vec) const {
    return !(*this == vec);
}

// Operations with Vec2D

inline Vec2D& Vec2D::operator+=(const Vec2D &vec) {
    _arr_point[0] += vec._arr_point[0];
    _arr_point[1] += vec._arr_point[1];
    return *this;
}

inline Vec2D& Vec2D::operator-=(const Vec2D &vec) {
    _arr_point[0] -= vec._arr_point[0];
    _arr_point[1] -= vec._arr_point[1];
    return *this;
}

inline Vec2D& Vec2D::operator*=(double number) {
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    return *this;
}

inline Vec2D& Vec2D::operator/=(double number) {
#ifndef NDEBUG
    if (std::abs(number) <= Consts::EPS) {
        throw std::domain_error{"Vec2D::operator/(double number): division by zero"};
    }
#endif
    _arr_point[0] /= number;
    _arr_point[1] /= number;
    return *this;
}


inline Vec2D Vec2D::operator+(const Vec2D &vec) const {
    Vec2D res = *this;
    res += vec;
    return res;
}

inline Vec2D Vec2D::operator-(const Vec2D &vec) const {
    Vec2D res = *this;
    res -= vec;
    return res;
}

inline Vec2D Vec2D::operator*(double number) const {
    Vec2D res = *this;
    res *= number;
    return res;
}

inline Vec2D Vec2D::operator/(double number) const {
    Vec2D res = *this;
    res /= number;
    return res;
}

// Other useful methods

inline double Vec2D::sqrAbs() const {
    return x() * x() + y() * y();
}

inline double Vec2D::abs() const {
    return sqrt(sqrAbs());
}

inline Vec2D Vec2D::normalized() const {
    double vecAbs = sqrAbs();
    if (vecAbs > Consts::EPS) {
        return *this / sqrt(vecAbs);
    } else {
        return Vec2D(0);
    }
}

inline double Vec2D::dot(const Vec2D &vec) const {
    return vec.x() * x() + vec.y() * y();
}

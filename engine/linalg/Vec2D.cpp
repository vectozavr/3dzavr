//
// Created by Иван Ильин on 10.10.2021.
//

#include <cmath>

#include <linalg/Vec2D.h>
#include <Consts.h>

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


Vec2D Vec2D::operator-() const &{
    return Vec2D(-x(), -y());
}

Vec2D &Vec2D::operator-() &&{
    _arr_point[0] = -_arr_point[0];
    _arr_point[1] = -_arr_point[1];
    return *this;
}


bool Vec2D::operator==(const Vec2D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

bool Vec2D::operator!=(const Vec2D &vec) const {
    return !(*this == vec);
}

// Operations with Vec2D

Vec2D &Vec2D::operator+=(const Vec2D &vec) {
    _arr_point[0] += vec._arr_point[0];
    _arr_point[1] += vec._arr_point[1];
    return *this;
}

Vec2D &Vec2D::operator-=(const Vec2D &vec) {
    _arr_point[0] -= vec._arr_point[0];
    _arr_point[1] -= vec._arr_point[1];
    return *this;
}

Vec2D &Vec2D::operator*=(double number) {
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    return *this;
}

Vec2D &Vec2D::operator/=(double number) {
#ifndef NDEBUG
    if (std::abs(number) <= Consts::EPS) {
        throw std::domain_error{"Color::operator/(double number): division by zero"};
    }
#endif
    _arr_point[0] /= number;
    _arr_point[1] /= number;
    return *this;
}


Vec2D Vec2D::operator+(const Vec2D &vec) const &{
    Vec2D res = *this;
    res += vec;
    return res;
}

Vec2D Vec2D::operator-(const Vec2D &vec) const &{
    Vec2D res = *this;
    res -= vec;
    return res;
}

Vec2D Vec2D::operator*(double number) const &{
    Vec2D res = *this;
    res *= number;
    return res;
}

Vec2D Vec2D::operator/(double number) const &{
    Vec2D res = *this;
    res /= number;
    return res;
}


Vec2D &Vec2D::operator+(const Vec2D &vec) &&{
    *this += vec;
    return *this;
}

Vec2D &Vec2D::operator-(const Vec2D &vec) &&{
    *this -= vec;
    return *this;
}

Vec2D &Vec2D::operator*(double number) &&{
    *this *= number;
    return *this;
}

Vec2D &Vec2D::operator/(double number) &&{
    *this /= number;
    return *this;
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

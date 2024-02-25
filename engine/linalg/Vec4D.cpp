//
// Created by Иван Ильин on 12.01.2021.
//

#include <cmath>
#include <stdexcept>

#include <linalg/Vec4D.h>
#include <Consts.h>

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


[[nodiscard]] Vec4D Vec4D::operator-() const &{
    return Vec4D(-x(), -y(), -z(), -w());
}

[[nodiscard]] Vec4D &Vec4D::operator-() &&{
    _arr_point[0] = -_arr_point[0];
    _arr_point[1] = -_arr_point[1];
    _arr_point[2] = -_arr_point[2];
    _arr_point[3] = -_arr_point[3];
    return *this;
}


bool Vec4D::operator==(const Vec4D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

bool Vec4D::operator!=(const Vec4D &vec) const {
    return !(*this == vec);
}

// Operations with Vec4D

Vec4D &Vec4D::operator+=(const Vec4D &vec) {
    _arr_point[0] += vec._arr_point[0];
    _arr_point[1] += vec._arr_point[1];
    _arr_point[2] += vec._arr_point[2];
    _arr_point[3] += vec._arr_point[3];
    return *this;
}

Vec4D &Vec4D::operator-=(const Vec4D &vec) {
    _arr_point[0] -= vec._arr_point[0];
    _arr_point[1] -= vec._arr_point[1];
    _arr_point[2] -= vec._arr_point[2];
    _arr_point[3] -= vec._arr_point[3];
    return *this;
}

Vec4D &Vec4D::operator*=(double number) {
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    _arr_point[2] *= number;
    _arr_point[3] *= number;
    return *this;
}

Vec4D &Vec4D::operator/=(double number) {
#ifndef NDEBUG
    if (std::abs(number) <= Consts::EPS) {
        throw std::domain_error{"Color::operator/(double number): division by zero"};
    }
#endif
    _arr_point[0] /= number;
    _arr_point[1] /= number;
    _arr_point[2] /= number;
    _arr_point[3] /= number;
    return *this;
}


Vec4D Vec4D::operator+(const Vec4D &vec) const &{
    Vec4D res = *this;
    res += vec;
    return res;
}

Vec4D Vec4D::operator-(const Vec4D &vec) const &{
    Vec4D res = *this;
    res -= vec;
    return res;
}

Vec4D Vec4D::operator*(double number) const &{
    Vec4D res = *this;
    res *= number;
    return res;
}

Vec4D Vec4D::operator/(double number) const &{
    Vec4D res = *this;
    res /= number;
    return res;
}


Vec4D &Vec4D::operator+(const Vec4D &vec) &&{
    *this += vec;
    return *this;
}

Vec4D &Vec4D::operator-(const Vec4D &vec) &&{
    *this -= vec;
    return *this;
}

Vec4D &Vec4D::operator*(double number) &&{
    *this *= number;
    return *this;
}

Vec4D &Vec4D::operator/(double number) &&{
    *this /= number;
    return *this;
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

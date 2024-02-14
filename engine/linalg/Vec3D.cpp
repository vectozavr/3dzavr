//
// Created by Иван Ильин on 09.10.2021.
//

#include <cmath>
#include <stdexcept>

#include <linalg/Vec3D.h>
#include <Consts.h>

Vec3D::Vec3D(const Vec3D &vec) {
    _arr_point[0] = vec.x();
    _arr_point[1] = vec.y();
    _arr_point[2] = vec.z();
}

Vec3D::Vec3D(const Vec4D &point4D) {
    _arr_point[0] = point4D.x();
    _arr_point[1] = point4D.y();
    _arr_point[2] = point4D.z();
}

Vec3D::Vec3D(double x, double y, double z) {
    _arr_point[0] = x;
    _arr_point[1] = y;
    _arr_point[2] = z;
}


Vec3D Vec3D::operator-() const &{
    return Vec3D(-x(), -y(), -z());
}

Vec3D &Vec3D::operator-() &&{
    _arr_point[0] = -_arr_point[0];
    _arr_point[1] = -_arr_point[1];
    _arr_point[2] = -_arr_point[2];
    return *this;
}


bool Vec3D::operator==(const Vec3D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

bool Vec3D::operator!=(const Vec3D &vec) const {
    return !(*this == vec);
}

// Operations with Vec3D

Vec3D &Vec3D::operator+=(const Vec3D &vec) {
    _arr_point[0] += vec._arr_point[0];
    _arr_point[1] += vec._arr_point[1];
    _arr_point[2] += vec._arr_point[2];
    return *this;
}

Vec3D &Vec3D::operator-=(const Vec3D &vec) {
    _arr_point[0] -= vec._arr_point[0];
    _arr_point[1] -= vec._arr_point[1];
    _arr_point[2] -= vec._arr_point[2];
    return *this;
}

Vec3D &Vec3D::operator*=(double number) {
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    _arr_point[2] *= number;
    return *this;
}

Vec3D &Vec3D::operator/=(double number) {
#ifndef NDEBUG
    if (std::abs(number) <= Consts::EPS) {
        throw std::domain_error{"Color::operator/(double number): division by zero"};
    }
#endif
    _arr_point[0] /= number;
    _arr_point[1] /= number;
    _arr_point[2] /= number;
    return *this;
}


Vec3D Vec3D::operator+(const Vec3D &vec) const &{
    Vec3D res = *this;
    res += vec;
    return res;
}

Vec3D Vec3D::operator-(const Vec3D &vec) const &{
    Vec3D res = *this;
    res -= vec;
    return res;
}

Vec3D Vec3D::operator*(double number) const &{
    Vec3D res = *this;
    res *= number;
    return res;
}

Vec3D Vec3D::operator/(double number) const &{
    Vec3D res = *this;
    res /= number;
    return res;
}


Vec3D &Vec3D::operator+(const Vec3D &vec) &&{
    *this += vec;
    return *this;
}

Vec3D &Vec3D::operator-(const Vec3D &vec) &&{
    *this -= vec;
    return *this;
}

Vec3D &Vec3D::operator*(double number) &&{
    *this *= number;
    return *this;
}

Vec3D &Vec3D::operator/(double number) &&{
    *this /= number;
    return *this;
}

// Other useful methods

double Vec3D::sqrAbs() const {
    return x() * x() + y() * y() + z() * z();
}

double Vec3D::abs() const {
    return sqrt(sqrAbs());
}

Vec3D Vec3D::normalized() const {
    double vecAbs = sqrAbs();
    if (vecAbs > Consts::EPS) {
        return *this / sqrt(vecAbs);
    } else {
        return Vec3D(1);
    }
}

double Vec3D::dot(const Vec3D &vec) const {
    return vec.x() * x() + vec.y() * y() + vec.z() * z();
}

Vec3D Vec3D::cross(const Vec3D &vec) const {
    return Vec3D{y() * vec.z() - vec.y() * z(),
                 z() * vec.x() - vec.z() * x(),
                 x() * vec.y() - vec.x() * y()};
}

Vec4D Vec3D::makePoint4D() const {
    return Vec4D(x(), y(), z(), 1.0);
}

Vec3D Vec3D::Random() {
    // TODO: use C++11 random library instead of Rand()
    return Vec3D((double) rand() / RAND_MAX, (double) rand() / RAND_MAX, (double) rand() / RAND_MAX);
}

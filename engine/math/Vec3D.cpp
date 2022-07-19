//
// Created by Иван Ильин on 09.10.2021.
//

#include <cmath>
#include <stdexcept>

#include "Vec3D.h"
#include "../Consts.h"

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

Vec3D Vec3D::operator-() const {
    return Vec3D(-x(), -y(), -z());
}

bool Vec3D::operator==(const Vec3D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

bool Vec3D::operator!=(const Vec3D &vec) const {
    return !(*this == vec);
}

// Operations with Vec3D
Vec3D Vec3D::operator+(const Vec3D &vec) const {
    return Vec3D(x() + vec.x(), y() + vec.y(), z() + vec.z());
}

Vec3D Vec3D::operator-(const Vec3D &vec) const {
    return *this + -vec;
}

Vec3D Vec3D::operator*(double number) const {
    return Vec3D(x() * number, y() * number, z() * number);
}

Vec3D Vec3D::operator/(double number) const {
    if (std::abs(number) > Consts::EPS) {
        return *this * (1.0 / number);
    } else {
        throw std::domain_error{"Vec3D::operator/(double number): division by zero"};
    }
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
    return Vec3D((double) rand() / RAND_MAX, (double) rand() / RAND_MAX, (double) rand() / RAND_MAX);
}

//
// Created by Иван Ильин on 12.01.2021.
//

#include "Point4D.h"
#include <cmath>

Point4D::Point4D (double x, double y, double z, double w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

[[nodiscard]] double Point4D::operator[] (int i) const {
    if(i == 0)
        return x;
    if(i == 1)
        return y;
    if(i == 2)
        return z;
    else
        return w;
}
[[nodiscard]] double& Point4D::operator[] (int i) {
    if(i == 0)
        return x;
    if(i == 1)
        return y;
    if(i == 2)
        return z;
    else
        return w;
}

[[nodiscard]] Point4D Point4D::operator-() const {
    return Point4D(-x, -y, -z, -w);
}
[[nodiscard]] Point4D Point4D::operator+() const {
    return Point4D(*this);
}

// Boolean operations
bool Point4D::operator==(const Point4D& point4D) const
{
    return this == &point4D || (*this - point4D).sqrAbs() < 0.0000000001;
}
bool Point4D::operator!=(const Point4D& point4D) const
{
    return this != &point4D && (*this - point4D).sqrAbs() > 0.0000000001;
}

// Operations with Point4D
Point4D Point4D::operator+(const Point4D& point4D) const
{
    return Point4D(*this) += point4D;
}
Point4D Point4D::operator-(const Point4D& point4D) const
{
    return Point4D(*this) -= point4D;
}
Point4D Point4D::operator*(const Point4D& point4D) const
{
    return Point4D(*this) *= point4D;
}
Point4D Point4D::operator/(const Point4D& point4D) const
{
    return Point4D(*this) /= point4D;
}
Point4D& Point4D::operator+=(const Point4D& point4D)
{
    this->x += point4D.x;
    this->y += point4D.y;
    this->z += point4D.x;
    this->w += point4D.y;
    return *this;
}
Point4D& Point4D::operator-=(const Point4D& point4D)
{
    (*this) += -point4D;
    return *this;
}
Point4D& Point4D::operator*=(const Point4D& point4D) {
    this->x *= point4D.x;
    this->y *= point4D.y;
    this->z *= point4D.z;
    this->w *= point4D.w;
    return *this;
}

Point4D& Point4D::operator/=(const Point4D& point4D) {
    this->x /= point4D.x;
    this->y /= point4D.y;
    this->z /= point4D.z;
    this->w /= point4D.w;
    return *this;
}
double Point4D::dot(const Point4D& point4D) const
{
    return point4D.x * x + point4D.y * y + point4D.z * z + point4D.w * w;
}
[[nodiscard]] double Point4D::dot3D(const Point4D& point4D) const {
    return point4D.x * x + point4D.y * y + point4D.z * z;
}
[[nodiscard]] Point4D Point4D::cross3D(const Point4D& point4D) const {
    Point4D cross{};
    cross.x = y * point4D.z - point4D.y * z;
    cross.y = z * point4D.x - point4D.z * x;
    cross.z = x * point4D.y - point4D.x * y;

    return cross;
}

// Operations with numbers
Point4D Point4D::operator+(double number) const {
    return Point4D(*this) += number;
}
Point4D Point4D::operator-(double number) const {
    return Point4D(*this) -= number;
}
Point4D Point4D::operator*(double number) const
{
    return Point4D(*this) *= number;
}
Point4D Point4D::operator/(double number) const
{
    return Point4D(*this) /= number;
}
Point4D& Point4D::operator+=(double number) {
    this->x += number;
    this->y += number;
    this->z += number;
    this->w += number;
    return *this;
}
Point4D& Point4D::operator-=(double number) {
    return *this += -number;
}
Point4D& Point4D::operator*=(double number)
{
    this->x *= number;
    this->y *= number;
    this->z *= number;
    this->w *= number;
    return *this;
}
Point4D& Point4D::operator/=(double number)
{
    return *this *= 1.0/number;
}

// Other useful methods
double Point4D::sqrAbs() const
{
    return x*x + y*y + z*z + w*w;
}
double Point4D::abs() const
{
    return sqrt(sqrAbs());
}
Point4D& Point4D::normalize()
{
    double length = this->abs();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
    return *this;
}

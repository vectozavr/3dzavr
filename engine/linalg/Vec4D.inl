#include <cmath>
#include <stdexcept>

#include <ScalarConsts.h>

inline Vec4D::Vec4D(double x, double y, double z, double w) : _arr_point{x, y, z, w} {}

inline Vec4D::Vec4D(const Vec4D &vec) : _arr_point{vec[0], vec[1], vec[2], vec[3]} {}


inline Vec4D Vec4D::operator-() const {
    return Vec4D(-x(), -y(), -z(), -w());
}


inline bool Vec4D::operator==(const Vec4D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

inline bool Vec4D::operator!=(const Vec4D &vec) const {
    return !(*this == vec);
}

// Operations with Vec4D

inline Vec4D &Vec4D::operator+=(const Vec4D &vec) {
    _arr_point[0] += vec._arr_point[0];
    _arr_point[1] += vec._arr_point[1];
    _arr_point[2] += vec._arr_point[2];
    _arr_point[3] += vec._arr_point[3];
    return *this;
}

inline Vec4D &Vec4D::operator-=(const Vec4D &vec) {
    _arr_point[0] -= vec._arr_point[0];
    _arr_point[1] -= vec._arr_point[1];
    _arr_point[2] -= vec._arr_point[2];
    _arr_point[3] -= vec._arr_point[3];
    return *this;
}

inline Vec4D &Vec4D::operator*=(double number) {
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    _arr_point[2] *= number;
    _arr_point[3] *= number;
    return *this;
}

inline Vec4D &Vec4D::operator/=(double number) {
#ifndef NDEBUG
    if (std::abs(number) <= Consts::EPS) {
        throw std::domain_error{"Vec4D::operator/(double number): division by zero"};
    }
#endif
    _arr_point[0] /= number;
    _arr_point[1] /= number;
    _arr_point[2] /= number;
    _arr_point[3] /= number;
    return *this;
}


inline Vec4D Vec4D::operator+(const Vec4D &vec) const {
    Vec4D res = *this;
    res += vec;
    return res;
}

inline Vec4D Vec4D::operator-(const Vec4D &vec) const {
    Vec4D res = *this;
    res -= vec;
    return res;
}

inline Vec4D Vec4D::operator*(double number) const {
    Vec4D res = *this;
    res *= number;
    return res;
}

inline Vec4D Vec4D::operator/(double number) const {
    Vec4D res = *this;
    res /= number;
    return res;
}

// Other useful methods

inline double Vec4D::sqrAbs() const {
    return x() * x() + y() * y() + z() * z() + w() * w();
}

inline double Vec4D::abs() const {
    return sqrt(sqrAbs());
}

inline Vec4D Vec4D::normalized() const {
    double vecAbs = sqrAbs();
    if (vecAbs > Consts::EPS) {
        return *this / sqrt(vecAbs);
    } else {
        return Vec4D(1);
    }
}

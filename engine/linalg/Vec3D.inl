#include <cmath>
#include <stdexcept>
#include <random>

#include <ScalarConsts.h>

inline Vec3D::Vec3D(const Vec3D &vec) : _arr_point{vec[0], vec[1], vec[2]} {}

inline Vec3D::Vec3D(const Vec4D &vec) : _arr_point{vec[0], vec[1], vec[2]} {}

inline Vec3D::Vec3D(double x, double y, double z) : _arr_point{x, y, z} {}


inline Vec3D Vec3D::operator-() const {
    return Vec3D(-x(), -y(), -z());
}


inline bool Vec3D::operator==(const Vec3D &vec) const {
    return (*this - vec).sqrAbs() < Consts::EPS;
}

inline bool Vec3D::operator!=(const Vec3D &vec) const {
    return !(*this == vec);
}

// Operations with Vec3D

inline Vec3D &Vec3D::operator+=(const Vec3D &vec) {
    _arr_point[0] += vec._arr_point[0];
    _arr_point[1] += vec._arr_point[1];
    _arr_point[2] += vec._arr_point[2];
    return *this;
}

inline Vec3D &Vec3D::operator-=(const Vec3D &vec) {
    _arr_point[0] -= vec._arr_point[0];
    _arr_point[1] -= vec._arr_point[1];
    _arr_point[2] -= vec._arr_point[2];
    return *this;
}

inline Vec3D &Vec3D::operator*=(double number) {
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    _arr_point[2] *= number;
    return *this;
}

inline Vec3D &Vec3D::operator/=(double number) {
#ifndef NDEBUG
    if (std::abs(number) <= Consts::EPS) {
        throw std::domain_error{"Vec3D::operator/(double number): division by zero"};
    }
#endif
    _arr_point[0] /= number;
    _arr_point[1] /= number;
    _arr_point[2] /= number;
    return *this;
}


inline Vec3D Vec3D::operator+(const Vec3D &vec) const {
    Vec3D res = *this;
    res += vec;
    return res;
}

inline Vec3D Vec3D::operator-(const Vec3D &vec) const {
    Vec3D res = *this;
    res -= vec;
    return res;
}

inline Vec3D Vec3D::operator*(double number) const {
    Vec3D res = *this;
    res *= number;
    return res;
}

inline Vec3D Vec3D::operator/(double number) const {
    Vec3D res = *this;
    res /= number;
    return res;
}

// Other useful methods

inline double Vec3D::sqrAbs() const {
    return x() * x() + y() * y() + z() * z();
}

inline double Vec3D::abs() const {
    return sqrt(sqrAbs());
}

inline Vec3D Vec3D::normalized() const {
    double vecAbs = sqrAbs();
    if (vecAbs > Consts::EPS) {
        return *this / sqrt(vecAbs);
    } else {
        return Vec3D(1);
    }
}

inline double Vec3D::dot(const Vec3D &vec) const {
    return vec.x() * x() + vec.y() * y() + vec.z() * z();
}

inline Vec3D Vec3D::cross(const Vec3D &vec) const {
    return Vec3D{y() * vec.z() - vec.y() * z(),
                 z() * vec.x() - vec.z() * x(),
                 x() * vec.y() - vec.x() * y()};
}

inline Vec4D Vec3D::makePoint4D() const {
    return Vec4D(x(), y(), z(), 1.0);
}

inline Vec3D Vec3D::Random() {
    // Create a static random engine with a random seed
    static std::mt19937 engine(std::random_device{}());

    // Create a uniform distribution in range [0, 1)
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Use the distribution and engine to generate three random doubles and return them as a Vec3D
    return Vec3D(dist(engine), dist(engine), dist(engine));
}

#include <cmath>

#include <ScalarConsts.h>
#include "Matrix3x3.h"

inline Matrix3x3::Matrix3x3(const Vec3D &column1, const Vec3D &column2, const Vec3D &column3) {
    _arr[0][0] = column1.x(); _arr[1][0] = column1.y(); _arr[2][0] = column1.z();
    _arr[0][1] = column2.x(); _arr[1][1] = column2.y(); _arr[2][1] = column2.z();
    _arr[0][2] = column3.x(); _arr[1][2] = column3.y(); _arr[2][2] = column3.z();
}

inline Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &matrix3X3) const {
    Matrix3x3 result = Matrix3x3::Zero();

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                result._arr[i][j] += _arr[i][k] * matrix3X3._arr[k][j];

    return result;
}

inline Vec3D Matrix3x3::operator*(const Vec3D &vec) const {
    return Vec3D(
            _arr[0][0] * vec.x() + _arr[0][1] * vec.y() + _arr[0][2] * vec.z(),
            _arr[1][0] * vec.x() + _arr[1][1] * vec.y() + _arr[1][2] * vec.z(),
            _arr[2][0] * vec.x() + _arr[2][1] * vec.y() + _arr[2][2] * vec.z()
    );
}

inline Matrix3x3 Matrix3x3::Identity() {
    Matrix3x3 result = Matrix3x3::Zero();

    result._arr[0][0] = 1.0;
    result._arr[1][1] = 1.0;
    result._arr[2][2] = 1.0;

    return result;
}

inline Matrix3x3 Matrix3x3::Constant(double value) {
    Matrix3x3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result._arr[j][i] = value;
        }
    }

    return result;
}

inline Matrix3x3 Matrix3x3::Zero() {
    return Matrix3x3::Constant(0);
}

inline Vec3D Matrix3x3::x() const {
    return Vec3D(_arr[0][0], _arr[1][0], _arr[2][0]);
}

inline Vec3D Matrix3x3::y() const {
    return Vec3D(_arr[0][1], _arr[1][1], _arr[2][1]);
}

inline Vec3D Matrix3x3::z() const {
    return Vec3D(_arr[0][2], _arr[1][2], _arr[2][2]);
}

inline std::ostream &operator<<(std::ostream &os, const Matrix3x3 &matrix3X3) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            os << matrix3X3._arr[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

inline double Matrix3x3::abs() const {
    double result = 0;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            result += _arr[i][j]*_arr[i][j];
        }
    }
    return std::sqrt(result);
}

inline Matrix3x3 Matrix3x3::operator+(const Matrix3x3 &matrix3X3) const {
    Matrix3x3 result = Matrix3x3::Zero();

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            result._arr[i][j] = _arr[i][j] + matrix3X3._arr[i][j];
        }
    }

    return result;
}

inline Matrix3x3 Matrix3x3::operator-(const Matrix3x3 &matrix3X3) const {
    Matrix3x3 result = Matrix3x3::Zero();

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            result._arr[i][j] = _arr[i][j] - matrix3X3._arr[i][j];
        }
    }

    return result;
}


inline Matrix3x3 Matrix3x3::inverse() const {
    std::array<std::array<double, 3>, 3> m = _arr;

    // computes the inverse of a matrix m
    double det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                 m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                 m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    Matrix3x3 inv = Matrix3x3::Zero();

    inv._arr[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) / det;
    inv._arr[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / det;
    inv._arr[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;
    inv._arr[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / det;
    inv._arr[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
    inv._arr[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) / det;
    inv._arr[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) / det;
    inv._arr[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) / det;
    inv._arr[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) / det;

    return inv;
}

inline Matrix3x3 Matrix3x3::Outer(const Vec3D &vec1, const Vec3D &vec2) {
    Matrix3x3 result;

    result._arr[0][0] = vec1.x()*vec2.x();
    result._arr[0][1] = vec1.x()*vec2.y();
    result._arr[0][2] = vec1.x()*vec2.z();

    result._arr[1][0] = vec1.y()*vec2.x();
    result._arr[1][1] = vec1.y()*vec2.y();
    result._arr[1][2] = vec1.y()*vec2.z();

    result._arr[2][0] = vec1.z()*vec2.x();
    result._arr[2][1] = vec1.z()*vec2.y();
    result._arr[2][2] = vec1.z()*vec2.z();

    return result;
}

inline Matrix3x3 Matrix3x3::operator*(double number) const {
    Matrix3x3 result(*this);

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            result._arr[i][j] = _arr[i][j]*number;
        }
    }

    return result;
}

inline Matrix3x3 &Matrix3x3::operator+=(const Matrix3x3 &matrix3x3) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            _arr[i][j] += matrix3x3._arr[i][j];
        }
    }

    return *this;
}

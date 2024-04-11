#include <cmath>

#include <ScalarConsts.h>
#include "Matrix3x3.h"

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

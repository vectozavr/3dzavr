//
// Created by Иван Ильин on 12.01.2021.
//

#include <cmath>

#include "Matrix4x4.h"
#include "../Consts.h"

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix4X4) const {
    Matrix4x4 result = Matrix4x4::Zero();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                result._arr[i][j] += _arr[i][k] * matrix4X4._arr[k][j];
    return result;
}

Vec4D Matrix4x4::operator*(const Vec4D &point4D) const {
    return Vec4D(
            _arr[0][0] * point4D.x() + _arr[0][1] * point4D.y() + _arr[0][2] * point4D.z() + _arr[0][3] * point4D.w(),
            _arr[1][0] * point4D.x() + _arr[1][1] * point4D.y() + _arr[1][2] * point4D.z() + _arr[1][3] * point4D.w(),
            _arr[2][0] * point4D.x() + _arr[2][1] * point4D.y() + _arr[2][2] * point4D.z() + _arr[2][3] * point4D.w(),
            _arr[3][0] * point4D.x() + _arr[3][1] * point4D.y() + _arr[3][2] * point4D.z() + _arr[3][3] * point4D.w()
    );
}

Vec3D Matrix4x4::operator*(const Vec3D &vec) const {
    return Vec3D(
            _arr[0][0] * vec.x() + _arr[0][1] * vec.y() + _arr[0][2] * vec.z(),
            _arr[1][0] * vec.x() + _arr[1][1] * vec.y() + _arr[1][2] * vec.z(),
            _arr[2][0] * vec.x() + _arr[2][1] * vec.y() + _arr[2][2] * vec.z()
    );
}

Matrix4x4 Matrix4x4::Identity() {
    Matrix4x4 result;

    result._arr[0][0] = 1.0;
    result._arr[1][1] = 1.0;
    result._arr[2][2] = 1.0;
    result._arr[3][3] = 1.0;

    return result;
}

Matrix4x4 Matrix4x4::Constant(double value) {
    Matrix4x4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result._arr[j][i] = value;
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::Zero() {
    return Matrix4x4::Constant(0);
}

Matrix4x4 Matrix4x4::Scale(const Vec3D &factor) {
    Matrix4x4 s{};
    s._arr[0][0] = factor.x();
    s._arr[1][1] = factor.y();
    s._arr[2][2] = factor.z();
    s._arr[3][3] = 1;

    return s;
}

Matrix4x4 Matrix4x4::Translation(const Vec3D &v) {
    Matrix4x4 t{};

    t._arr[0][0] = 1.0;
    t._arr[1][1] = 1.0;
    t._arr[2][2] = 1.0;
    t._arr[3][3] = 1.0;

    t._arr[0][3] = v.x();
    t._arr[1][3] = v.y();
    t._arr[2][3] = v.z();

    return t;
}

Matrix4x4 Matrix4x4::RotationX(double rx) {
    Matrix4x4 Rx{};

    double c = cos(rx), s = sin(rx);

    Rx._arr[0][0] = 1.0;

    Rx._arr[1][1] = c;
    Rx._arr[1][2] = -s;
    Rx._arr[2][1] = s;
    Rx._arr[2][2] = c;

    Rx._arr[3][3] = 1.0;

    return Rx;
}

Matrix4x4 Matrix4x4::RotationY(double ry) {
    Matrix4x4 Ry{};

    double c = cos(ry), s = sin(ry);

    Ry._arr[1][1] = 1.0;

    Ry._arr[0][0] = c;
    Ry._arr[0][2] = s;
    Ry._arr[2][0] = -s;
    Ry._arr[2][2] = c;

    Ry._arr[3][3] = 1.0;

    return Ry;
}

Matrix4x4 Matrix4x4::RotationZ(double rz) {
    Matrix4x4 Rz{};

    double c = cos(rz), s = sin(rz);

    Rz._arr[2][2] = 1.0;

    Rz._arr[0][0] = c;
    Rz._arr[0][1] = -s;
    Rz._arr[1][0] = s;
    Rz._arr[1][1] = c;

    Rz._arr[3][3] = 1.0;

    return Rz;
}

Matrix4x4 Matrix4x4::Rotation(const Vec3D &r) {
    return RotationX(r.x()) * RotationY(r.y()) * RotationZ(r.z());
}

Matrix4x4 Matrix4x4::Rotation(const Vec3D &v, double rv) {
    Matrix4x4 Rv{};
    Vec3D nv(v.normalized());

    double c = cos(rv), s = sin(rv);

    Rv._arr[0][0] = c + (1.0 - c) * nv.x() * nv.x();
    Rv._arr[0][1] = (1.0 - c) * nv.x() * nv.y() - s * nv.z();
    Rv._arr[0][2] = (1.0 - c) * nv.x() * nv.z() + s * nv.y();

    Rv._arr[1][0] = (1.0 - c) * nv.x() * nv.y() + s * nv.z();
    Rv._arr[1][1] = c + (1.0 - c) * nv.y() * nv.y();
    Rv._arr[1][2] = (1.0 - c) * nv.y() * nv.z() - s * nv.x();

    Rv._arr[2][0] = (1.0 - c) * nv.z() * nv.x() - s * nv.y();
    Rv._arr[2][1] = (1.0 - c) * nv.z() * nv.y() + s * nv.x();
    Rv._arr[2][2] = c + (1.0 - c) * nv.z() * nv.z();

    Rv._arr[3][3] = 1.0;

    return Rv;
}

Matrix4x4 Matrix4x4::Projection(double fov, double aspect, double ZNear, double ZFar) {
    Matrix4x4 p{};

    p._arr[0][0] = 1.0 / (tan(Consts::PI * fov * 0.5 / 180.0) * aspect);
    p._arr[1][1] = 1.0 / tan(Consts::PI * fov * 0.5 / 180.0);
    p._arr[2][2] = ZFar / (ZFar - ZNear);
    p._arr[2][3] = -ZFar * ZNear / (ZFar - ZNear);
    p._arr[3][2] = 1.0;

    return p;
}

Matrix4x4 Matrix4x4::ScreenSpace(int width, int height) {
    Matrix4x4 s{};

    s._arr[0][0] = -0.5 * width;
    s._arr[1][1] = -0.5 * height;
    s._arr[2][2] = 1.0;

    s._arr[0][3] = 0.5 * width;
    s._arr[1][3] = 0.5 * height;

    s._arr[3][3] = 1.0;

    return s;
}

Matrix4x4 Matrix4x4::View(const Matrix4x4 &transformMatrix) {
    Matrix4x4 V = Zero();

    Vec3D left      = transformMatrix.x();
    Vec3D up        = transformMatrix.y();
    Vec3D lookAt    = transformMatrix.z();
    Vec3D eye       = transformMatrix.w();

    double left_sqrAbs      = left.sqrAbs();
    double up_sqrAbs        = up.sqrAbs();
    double lookAt_sqrAbs    = lookAt.sqrAbs();

    V._arr[0][0] = left.x()/left_sqrAbs;
    V._arr[0][1] = left.y()/left_sqrAbs;
    V._arr[0][2] = left.z()/left_sqrAbs;
    V._arr[0][3] = -eye.dot(left)/left_sqrAbs;

    V._arr[1][0] = up.x()/up_sqrAbs;
    V._arr[1][1] = up.y()/up_sqrAbs;
    V._arr[1][2] = up.z()/up_sqrAbs;
    V._arr[1][3] = -eye.dot(up)/up_sqrAbs;

    V._arr[2][0] = lookAt.x()/lookAt_sqrAbs;
    V._arr[2][1] = lookAt.y()/lookAt_sqrAbs;
    V._arr[2][2] = lookAt.z()/lookAt_sqrAbs;
    V._arr[2][3] = -eye.dot(lookAt)/lookAt_sqrAbs;

    V._arr[3][3] = 1.0;

    return V;
}

Vec3D Matrix4x4::x() const {
    return Vec3D(_arr[0][0], _arr[1][0], _arr[2][0]);
}

Vec3D Matrix4x4::y() const {
    return Vec3D(_arr[0][1], _arr[1][1], _arr[2][1]);
}

Vec3D Matrix4x4::z() const {
    return Vec3D(_arr[0][2], _arr[1][2], _arr[2][2]);
}

Vec3D Matrix4x4::w() const {
    return Vec3D(_arr[0][3], _arr[1][3], _arr[2][3]);
}

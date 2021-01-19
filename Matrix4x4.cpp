//
// Created by Иван Ильин on 12.01.2021.
//

#include "Matrix4x4.h"
#include <cassert>
#include <cmath>

[[nodiscard]] const std::array<double, 4>& Matrix4x4::operator[] (int i) const {
    return arr_matrix[i];
}

std::array<double, 4> &Matrix4x4::operator[](int i) {
    return arr_matrix[i];
}

[[nodiscard]] Matrix4x4 Matrix4x4::operator-() const {
    Matrix4x4 result(*this);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result[i][j] = -arr_matrix[i][j];

    return result;
}

[[nodiscard]] Matrix4x4 Matrix4x4::operator+() const {
    return Matrix4x4(*this);
}

bool Matrix4x4::operator==(const Matrix4x4 &matrix4X4) const {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(arr_matrix[i][j] != matrix4X4[i][j])
                return false;
    return true;
}

bool Matrix4x4::operator!=(const Matrix4x4 &matrix4X4) const {
    return !(*this==matrix4X4);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &matrix4X4) const {
    return Matrix4x4(*this) += matrix4X4;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &matrix4X4) const {
    return Matrix4x4(*this) -= matrix4X4;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix4X4) const {
    return Matrix4x4(*this) *= matrix4X4;
}

Matrix4x4 Matrix4x4::operator/(const Matrix4x4 &matrix4X4) const {
    return Matrix4x4(*this) /= matrix4X4;
}

Matrix4x4 &Matrix4x4::operator+=(const Matrix4x4 &matrix4X4) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            arr_matrix[i][j] += matrix4X4[i][j];

    return *this;
}

Matrix4x4 &Matrix4x4::operator-=(const Matrix4x4 &matrix4X4) {
    (*this) += -matrix4X4;
    return *this;
}

Matrix4x4 &Matrix4x4::operator*=(const Matrix4x4 &matrix4X4) {
    Matrix4x4 copy(*this);
    this->setZero();

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int k = 0; k < 4; k++)
                arr_matrix[i][j] += copy[i][k] * matrix4X4[k][j];
    return *this;
}

Matrix4x4 &Matrix4x4::operator/=(const Matrix4x4 &matrix4X4) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) {
            assert(matrix4X4[i][j] != 0);
            arr_matrix[i][j] /= matrix4X4[i][j];
        }
    return *this;
}

Matrix4x4 Matrix4x4::operator+(double number) const {
    return Matrix4x4(*this) += number;
}

Matrix4x4 Matrix4x4::operator-(double number) const {
    return *this+(-number);
}

Matrix4x4 Matrix4x4::operator*(double number) const {
    return Matrix4x4(*this) *= number;
}

Matrix4x4 Matrix4x4::operator/(double number) const {
    assert(number != 0);
    return *this*(1.0/number);
}

Matrix4x4 &Matrix4x4::operator+=(double number) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            arr_matrix[i][j] += number;

    return *this;
}

Matrix4x4 &Matrix4x4::operator-=(double number) {
    return (*this) += -number;
}

Matrix4x4 &Matrix4x4::operator*=(double number) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            arr_matrix[i][j] *= number;

    return *this;
}

Matrix4x4 &Matrix4x4::operator/=(double number) {
    return (*this) *= 1.0/number;
}

Point4D Matrix4x4::operator*(const Point4D &point4D) const {
    Point4D result{};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result[i] += arr_matrix[i][j] * point4D[j];
    return result;
}

double Matrix4x4::det3D() {
    return  arr_matrix[0][0] * (arr_matrix[1][1] * arr_matrix[2][2] - arr_matrix[1][2] * arr_matrix[2][1]) -
            arr_matrix[0][1] * (arr_matrix[1][0] * arr_matrix[2][2] - arr_matrix[1][2] * arr_matrix[2][0]) +
            arr_matrix[0][2] * (arr_matrix[1][0] * arr_matrix[2][1] - arr_matrix[2][0] * arr_matrix[1][1]);
}

Matrix4x4 Matrix4x4::inv3D() {
    double det = det3D();
    assert(det != 0);
    Matrix4x4 result{};
    result[3][3] = 1.0;
    result[0][0] = arr_matrix[1][1] * arr_matrix[2][2] - arr_matrix[2][1] * arr_matrix[1][2];
    result[1][0] = arr_matrix[0][1] * arr_matrix[2][2] - arr_matrix[2][1] * arr_matrix[0][2];
    result[2][0] = arr_matrix[0][1] * arr_matrix[1][2] - arr_matrix[1][1] * arr_matrix[0][2];

    result[0][1] = arr_matrix[1][0] * arr_matrix[2][2] - arr_matrix[2][0] * arr_matrix[1][2];
    result[1][1] = arr_matrix[0][0] * arr_matrix[2][2] - arr_matrix[2][0] * arr_matrix[0][2];
    result[2][1] = arr_matrix[0][0] * arr_matrix[1][2] - arr_matrix[1][0] * arr_matrix[0][2];

    result[0][2] = arr_matrix[1][0] * arr_matrix[2][1] - arr_matrix[2][0] * arr_matrix[1][1];
    result[1][2] = arr_matrix[0][0] * arr_matrix[2][1] - arr_matrix[2][0] * arr_matrix[0][1];
    result[2][2] = arr_matrix[0][0] * arr_matrix[1][1] - arr_matrix[1][0] * arr_matrix[0][1];

    return result /= det;
}

Matrix4x4 &Matrix4x4::transpose() {
    Matrix4x4 result{};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result[i][j] = arr_matrix[j][i];

    *this = result;
    return *this;
}

Matrix4x4 &Matrix4x4::setIdentity() {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(i == j)
                arr_matrix[j][i] = 1.0;
            else
                arr_matrix[j][i] = 0.0;
    return *this;
}

Matrix4x4 &Matrix4x4::setOnes() {
    setConstants(1.0);
    return *this;
}

Matrix4x4 &Matrix4x4::setZero() {
    setConstants(0.0);
    return *this;
}

Matrix4x4 &Matrix4x4::setConstants(double value) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            arr_matrix[j][i] = value;
    return *this;
}

const std::array<std::array<double, 4>, 4> &Matrix4x4::data() const {
    return arr_matrix;
}

std::array<std::array<double, 4>, 4> &Matrix4x4::data() {
    return arr_matrix;
}

Matrix4x4 Matrix4x4::Identity() {
    return Matrix4x4().setIdentity();
}

Matrix4x4 Matrix4x4::Zero() {
    return Matrix4x4();
}

Matrix4x4 Matrix4x4::Constant(double value) {
    return Matrix4x4().setConstants(value);
}

Matrix4x4 Matrix4x4::Scale(double sx, double sy, double sz) {
    Matrix4x4 s{};
    s[0][0] = sx;
    s[1][1] = sy;
    s[2][2] = sz;
    s[3][3] = 1;

    return s;
}

Matrix4x4 Matrix4x4::Translation(double dx, double dy, double dz) {
    Matrix4x4 t{};
/*
 * ( 1 0 0 dx )(x)   (x + dx)
 * ( 0 1 0 dy )(y) = (y + dy)
 * ( 0 0 1 dz )(z)   (z + dz)
 * ( 0 0 0 1  )(1)   (  1   )
 */

    t[0][0] = 1.0;
    t[1][1] = 1.0;
    t[2][2] = 1.0;
    t[3][3] = 1.0;

    t[0][3] = dx;
    t[1][3] = dy;
    t[2][3] = dz;

    return t;
}

Matrix4x4 Matrix4x4::Translation(const Point4D& v) {
    Matrix4x4 t{};
/*
 * ( 1 0 0 dx )(x)   (x + dx)
 * ( 0 1 0 dy )(y) = (y + dy)
 * ( 0 0 1 dz )(z)   (z + dz)
 * ( 0 0 0 1  )(1)   (  1   )
 */

    t[0][0] = 1.0;
    t[1][1] = 1.0;
    t[2][2] = 1.0;
    t[3][3] = 1.0;

    t[0][3] = v.x;
    t[1][3] = v.y;
    t[2][3] = v.z;

    return t;
}

Matrix4x4 Matrix4x4::RotationX(double rx) {
    Matrix4x4 Rx{};
    Rx[0][0] = 1.0;

    Rx[1][1] = cos(rx);
    Rx[1][2] = -sin(rx);
    Rx[2][1] = sin(rx);
    Rx[2][2] = cos(rx);

    Rx[3][3] = 1.0;

    return Rx;
}

Matrix4x4 Matrix4x4::RotationY(double ry) {
    Matrix4x4 Ry{};

    Ry[1][1] = 1.0;

    Ry[0][0] = cos(ry);
    Ry[0][2] = sin(ry);
    Ry[2][0] = -sin(ry);
    Ry[2][2] = cos(ry);

    Ry[3][3] = 1.0;

    return Ry;
}

Matrix4x4 Matrix4x4::RotationZ(double rz) {
    Matrix4x4 Rz{};

    Rz[2][2] = 1.0;

    Rz[0][0] = cos(rz);
    Rz[0][1] = -sin(rz);
    Rz[1][0] = sin(rz);
    Rz[1][1] = cos(rz);

    Rz[3][3] = 1.0;

    return Rz;
}

Matrix4x4 Matrix4x4::Rotation(double rx, double ry, double rz) {
    return RotationX(rx) * RotationY(ry) * RotationZ(rz);
}

Matrix4x4 Matrix4x4::Rotation(Point4D v, double rv) {
    Matrix4x4 Rv{};
    v.normalize();

    Rv[0][0] = cos(rv) + (1.0 - cos(rv))*v.x*v.x;
    Rv[0][1] = (1.0 - cos(rv))*v.x*v.y - sin(rv)*v.z;
    Rv[0][2] = (1.0 - cos(rv))*v.x*v.z + sin(rv)*v.y;

    Rv[1][0] = (1.0 - cos(rv))*v.x*v.y + sin(rv)*v.z;
    Rv[1][1] = cos(rv) + (1.0 - cos(rv))*v.y*v.y;
    Rv[1][2] = (1.0 - cos(rv))*v.y*v.z - sin(rv)*v.x;

    Rv[2][0] = (1.0 - cos(rv))*v.z*v.x - sin(rv)*v.y;
    Rv[2][1] = (1.0 - cos(rv))*v.z*v.y + sin(rv)*v.x;
    Rv[2][2] = cos(rv) + (1.0 - cos(rv))*v.z*v.z;

    Rv[3][3] = 1.0;

    return Rv;
}



Matrix4x4 Matrix4x4::Projection(double fov, double aspect, double ZNear, double ZFar) {
    Matrix4x4 p{};

    p[0][0] = 1.0/(tan(M_PI*fov*0.5/180.0)*aspect);
    p[1][1] = 1.0/tan(M_PI*fov*0.5/180.0);
    p[2][2] = ZFar/(ZFar - ZNear);
    p[2][3] = -ZFar*ZNear/(ZFar - ZNear);
    p[3][2] = 1.0;

    return p;
}

Matrix4x4 Matrix4x4::ScreenSpace(int width, int height) {
    Matrix4x4 s{};

    s[0][0] = -0.5*width;
    s[1][1] = -0.5*height;
    s[2][2] = 1.0;

    s[0][3] = 0.5*width;
    s[1][3] = 0.5*height;

    s[3][3] = 1.0;

    return s;
}

Matrix4x4 Matrix4x4::View(const Point4D &left, const Point4D &up, const Point4D &lookAt, const Point4D &eye) {
    Matrix4x4 V{};

    V.setZero();

    V[0][0] = left[0];
    V[0][1] = left[1];
    V[0][2] = left[2];
    V[0][3] = -eye.dot(left);

    V[1][0] = up[0];
    V[1][1] = up[1];
    V[1][2] = up[2];
    V[1][3] = -eye.dot(up);

    V[2][0] = lookAt[0];
    V[2][1] = lookAt[1];
    V[2][2] = lookAt[2];
    V[2][3] = -eye.dot(lookAt);

    V[3][3] = 1.0;

    return V;
}

Matrix4x4 Matrix4x4::ViewInverse(const Point4D &left, const Point4D &up, const Point4D &lookAt, const Point4D &eye) {
    Matrix4x4 inv{};

    inv.setZero();

    inv[0][0] = left[0];
    inv[1][0] = left[1];
    inv[2][0] = left[2];
    inv[0][3] = eye[0];

    inv[0][1] = up[0];
    inv[1][1] = up[1];
    inv[2][1] = up[2];
    inv[1][3] = eye[1];

    inv[0][2] = lookAt[0];
    inv[1][2] = lookAt[1];
    inv[2][2] = lookAt[2];
    inv[2][3] = eye[2];

    inv[3][3] = 1.0;

    return inv;
}

Matrix4x4 Matrix4x4::Rotation(const Point4D &v) {
    return RotationX(v.x)*RotationY(v.y)*RotationZ(v.z);
}

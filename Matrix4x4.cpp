//
// Created by Иван Ильин on 12.01.2021.
//

#include "Matrix4x4.h"

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
    t[0][3] = dx;
    t[0][3] = dy;
    t[0][3] = dz;
    t[3][3] = 1;

    return t;
}

Matrix4x4 Matrix4x4::RotationX(double rx) {
    Matrix4x4 Rx{};
    Rx[0][0] = 1;

    Rx[1][1] = cos(rx);
    Rx[1][2] = -sin(rx);
    Rx[2][1] = sin(rx);
    Rx[2][2] = cos(rx);

    Rx[3][3] = 1;

    return Rx;
}

Matrix4x4 Matrix4x4::RotationY(double ry) {
    Matrix4x4 Ry{};

    Ry[1][1] = 1;

    Ry[0][0] = cos(ry);
    Ry[0][2] = sin(ry);
    Ry[2][0] = -sin(ry);
    Ry[2][2] = cos(ry);

    Ry[3][3] = 1;

    return Ry;
}

Matrix4x4 Matrix4x4::RotationZ(double rz) {
    Matrix4x4 Rz{};

    Rz[2][2] = 1;

    Rz[0][0] = cos(rz);
    Rz[0][1] = -sin(rz);
    Rz[1][0] = sin(rz);
    Rz[1][1] = cos(rz);

    Rz[3][3] = 1;

    return Rz;
}

Matrix4x4 Matrix4x4::Rotation(double rx, double ry, double rz) {
    return RotationX(rx) * RotationY(ry) * RotationZ(rz);
}

Matrix4x4 Matrix4x4::Rotation(Point4D v, double rv) {
    Matrix4x4 Rv{};
    v.normalize();

    Rv[0][0] = cos(rv) + (1 - cos(rv))*v.x*v.x;
    Rv[0][1] = (1 - cos(rv))*v.x*v.y - sin(rv)*v.z;
    Rv[0][2] = (1 - cos(rv))*v.x*v.z + sin(rv)*v.y;

    Rv[1][0] = (1 - cos(rv))*v.x*v.y + sin(rv)*v.z;
    Rv[1][1] = cos(rv) + (1 - cos(rv))*v.y*v.y;
    Rv[1][2] = (1 - cos(rv))*v.y*v.z - sin(rv)*v.x;

    Rv[2][0] = (1 - cos(rv))*v.z*v.x - sin(rv)*v.y;
    Rv[2][1] = (1 - cos(rv))*v.z*v.y + sin(rv)*v.x;
    Rv[2][2] = cos(rv) + (1 - cos(rv))*v.z*v.z;

    Rv[3][3] = 1;

    return Rv;
}

Matrix4x4 Matrix4x4::Projection(double fov, double aspect, double ZNear, double ZFar) {
    Matrix4x4 p{};

    p[0][0] = aspect*1.0/tan(M_PI*fov/180.0);
    p[1][1] = 1.0/tan(M_PI*fov/180.0);
    p[2][2] = ZFar/(ZFar - ZNear);
    p[2][3] = -ZFar*ZNear/(ZFar - ZNear);
    p[3][2] = 1;

    return p;
}

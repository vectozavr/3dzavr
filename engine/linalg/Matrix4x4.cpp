//
// Created by Иван Ильин on 12.01.2021.
//

#include <cmath>

#include <linalg/Matrix4x4.h>
#include <Consts.h>

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

    Vec3D left   = transformMatrix.x();
    Vec3D up     = transformMatrix.y();
    Vec3D lookAt = transformMatrix.z();
    Vec3D eye    = transformMatrix.w();

    double left_sqrAbs   = left.sqrAbs();
    double up_sqrAbs     = up.sqrAbs();
    double lookAt_sqrAbs = lookAt.sqrAbs();

    // TODO: what is the optimal EPS here? If EPS will be too small -> we will not use fast calculations.
    //  If the EPS is large -> operations might become unstable.
    if((std::abs(left.dot(up)) < Consts::EPS) &&
       (std::abs(left.dot(lookAt)) < Consts::EPS) &&
       (std::abs(up.dot(lookAt)) < Consts::EPS)) {
        // When left, up and lookAt are perpendicular to each other

        V._arr[0][0] = left.x()/left_sqrAbs;
        V._arr[0][1] = left.y()/left_sqrAbs;
        V._arr[0][2] = left.z()/left_sqrAbs;

        V._arr[1][0] = up.x()/up_sqrAbs;
        V._arr[1][1] = up.y()/up_sqrAbs;
        V._arr[1][2] = up.z()/up_sqrAbs;

        V._arr[2][0] = lookAt.x()/lookAt_sqrAbs;
        V._arr[2][1] = lookAt.y()/lookAt_sqrAbs;
        V._arr[2][2] = lookAt.z()/lookAt_sqrAbs;

    } else {
        // General case: up and lookAt are not necessary perpendicular

        std::array<std::array<double, 4>, 4> m = transformMatrix._arr;

        double det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                     m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                     m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

        V._arr[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) / det;
        V._arr[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / det;
        V._arr[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;

        V._arr[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / det;
        V._arr[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
        V._arr[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) / det;

        V._arr[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) / det;
        V._arr[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) / det;
        V._arr[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) / det;
    }

    V._arr[0][3] = -eye.dot(left)/left_sqrAbs;
    V._arr[1][3] = -eye.dot(up)/up_sqrAbs;
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

std::ostream &operator<<(std::ostream &os, const Matrix4x4 &matrix4x4) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            os << matrix4x4._arr[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

double Matrix4x4::abs() const {
    double result = 0;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result += _arr[i][j]*_arr[i][j];
        }
    }
    return std::sqrt(result);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &matrix4X4) const {
    Matrix4x4 result = Matrix4x4::Zero();

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result._arr[i][j] = _arr[i][j] + matrix4X4._arr[i][j];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &matrix4X4) const {
    Matrix4x4 result = Matrix4x4::Zero();

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result._arr[i][j] = _arr[i][j] - matrix4X4._arr[i][j];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::inverse() const {

    std::array<std::array<double, 4>, 4> m = _arr;

    double A2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    double A1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    double A1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    double A0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    double A0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    double A0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    double A2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
    double A1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
    double A1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
    double A2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
    double A1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
    double A1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    double A0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
    double A0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
    double A0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
    double A0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
    double A0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
    double A0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

    double det =
            m[0][0] * ( m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223 )
          - m[0][1] * ( m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223 )
          + m[0][2] * ( m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123 )
          - m[0][3] * ( m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123 );

    if(std::abs(det) < Consts::EPS) {
        throw std::domain_error{"Matrix4x4::inverse(): singular matrix."};
    }

    det = 1.0 / det;

    Matrix4x4 inv = Matrix4x4::Zero();

    inv._arr[0][0] = det *   ( m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223 );
    inv._arr[0][1] = det * - ( m[0][1] * A2323 - m[0][2] * A1323 + m[0][3] * A1223 );
    inv._arr[0][2] = det *   ( m[0][1] * A2313 - m[0][2] * A1313 + m[0][3] * A1213 );
    inv._arr[0][3] = det * - ( m[0][1] * A2312 - m[0][2] * A1312 + m[0][3] * A1212 );
    inv._arr[1][0] = det * - ( m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223 );
    inv._arr[1][1] = det *   ( m[0][0] * A2323 - m[0][2] * A0323 + m[0][3] * A0223 );
    inv._arr[1][2] = det * - ( m[0][0] * A2313 - m[0][2] * A0313 + m[0][3] * A0213 );
    inv._arr[1][3] = det *   ( m[0][0] * A2312 - m[0][2] * A0312 + m[0][3] * A0212 );
    inv._arr[2][0] = det *   ( m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123 );
    inv._arr[2][1] = det * - ( m[0][0] * A1323 - m[0][1] * A0323 + m[0][3] * A0123 );
    inv._arr[2][2] = det *   ( m[0][0] * A1313 - m[0][1] * A0313 + m[0][3] * A0113 );
    inv._arr[2][3] = det * - ( m[0][0] * A1312 - m[0][1] * A0312 + m[0][3] * A0112 );
    inv._arr[3][0] = det * - ( m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123 );
    inv._arr[3][1] = det *   ( m[0][0] * A1223 - m[0][1] * A0223 + m[0][2] * A0123 );
    inv._arr[3][2] = det * - ( m[0][0] * A1213 - m[0][1] * A0213 + m[0][2] * A0113 );
    inv._arr[3][3] = det *   ( m[0][0] * A1212 - m[0][1] * A0212 + m[0][2] * A0112 );

    return inv;
}

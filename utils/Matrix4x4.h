//
// Created by Иван Ильин on 12.01.2021.
//

#ifndef INC_3DZAVR_MATRIX4X4_H
#define INC_3DZAVR_MATRIX4X4_H

#include <array>
#include "Point4D.h"

class Matrix4x4 {
private:
    std::array<std::array<double, 4>, 4> arr_matrix{};
public:
    Matrix4x4 () = default;
    Matrix4x4& operator=(const Matrix4x4& matrix4X4) = default;

    [[nodiscard]] const std::array<double, 4>& operator[] (int i) const;
    [[nodiscard]] std::array<double, 4>& operator[] (int i);

    [[nodiscard]] Matrix4x4 operator-() const;
    [[nodiscard]] Matrix4x4 operator+() const;

    // Boolean operations
    bool operator==(const Matrix4x4& matrix4X4) const;
    bool operator!=(const Matrix4x4& matrix4X4) const;

    // Operations with Matrix4x4
    [[nodiscard]] Matrix4x4 operator+(const Matrix4x4& matrix4X4) const;
    [[nodiscard]] Matrix4x4 operator-(const Matrix4x4& matrix4X4) const;
    [[nodiscard]] Matrix4x4 operator*(const Matrix4x4& matrix4X4) const;
    [[nodiscard]] Matrix4x4 operator/(const Matrix4x4& matrix4X4) const;

    Matrix4x4& operator+=(const Matrix4x4& matrix4X4);
    Matrix4x4& operator-=(const Matrix4x4& matrix4X4);
    Matrix4x4& operator*=(const Matrix4x4& matrix4X4);
    Matrix4x4& operator/=(const Matrix4x4& matrix4X4);

    // Operations with numbers
    [[nodiscard]] Matrix4x4 operator+(double number) const;
    [[nodiscard]] Matrix4x4 operator-(double number) const;
    [[nodiscard]] Matrix4x4 operator*(double number) const;
    [[nodiscard]] Matrix4x4 operator/(double number) const;

    Matrix4x4& operator+=(double number);
    Matrix4x4& operator-=(double number);
    Matrix4x4& operator*=(double number);
    Matrix4x4& operator/=(double number);

    // Operations with Point4D
    [[nodiscard]] Point4D operator*(const Point4D& point4D) const;

    // Other useful methods
    [[nodiscard]] double det3D();
    [[nodiscard]] Matrix4x4 inv3D();
    Matrix4x4& transpose();
    Matrix4x4& setIdentity();
    Matrix4x4& setOnes();
    Matrix4x4& setZero();
    Matrix4x4& setConstants(double value);

    [[nodiscard]] const std::array<std::array<double, 4>, 4> & data() const;
    [[nodiscard]] std::array<std::array<double, 4>, 4> & data();

    // Any useful matrix
    Matrix4x4 static Identity();
    Matrix4x4 static Zero();
    Matrix4x4 static Constant (double value);

    Matrix4x4 static Scale(double sx, double sy, double sz);
    Matrix4x4 static Scale(const Point4D& s);
    Matrix4x4 static Translation(double dx, double dy, double dz);
    Matrix4x4 static Translation(const Point4D& v);
    Matrix4x4 static Rotation(const Point4D& r);
    Matrix4x4 static RotationX (double rx);
    Matrix4x4 static RotationY (double ry);
    Matrix4x4 static RotationZ (double rz);
    Matrix4x4 static Rotation (double rx, double ry, double rz);
    Matrix4x4 static Rotation (Point4D v, double rv);

    Matrix4x4 static View(const Point4D& left, const Point4D& up, const Point4D& lookAt, const Point4D& eye);
    Matrix4x4 static ViewInverse(const Point4D& left, const Point4D& up, const Point4D& lookAt, const Point4D& eye);
    Matrix4x4 static Projection (double fov = 90.0, double aspect = 1.0, double ZNear = 1.0, double ZFar = 10.0);
    Matrix4x4 static ScreenSpace (int width, int height);
};


#endif //INC_3DZAVR_MATRIX4X4_H

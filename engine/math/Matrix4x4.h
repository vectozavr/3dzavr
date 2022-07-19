//
// Created by Иван Ильин on 12.01.2021.
//

#ifndef ENGINE_MATRIX4X4_H
#define ENGINE_MATRIX4X4_H

#include <array>

#include "Vec4D.h"
#include "Vec3D.h"

class Matrix4x4 final {
private:
    std::array<std::array<double, 4>, 4> _arr{};

public:
    Matrix4x4() = default;

    Matrix4x4 &operator=(const Matrix4x4 &matrix4X4) = default;

    [[nodiscard]] Matrix4x4 operator*(const Matrix4x4 &matrix4X4) const;

    [[nodiscard]] Vec4D operator*(const Vec4D &point4D) const;

    [[nodiscard]] Vec3D operator*(const Vec3D &vec) const;

    [[nodiscard]] Vec3D x() const;

    [[nodiscard]] Vec3D y() const;

    [[nodiscard]] Vec3D z() const;

    [[nodiscard]] Vec3D w() const;


    // Any useful matrix (static methods)
    Matrix4x4 static Identity();

    Matrix4x4 static Zero();

    Matrix4x4 static Constant(double value);

    Matrix4x4 static Scale(const Vec3D &factor);

    Matrix4x4 static Translation(const Vec3D &v);

    Matrix4x4 static Rotation(const Vec3D &r);

    Matrix4x4 static RotationX(double rx);

    Matrix4x4 static RotationY(double ry);

    Matrix4x4 static RotationZ(double rz);

    Matrix4x4 static Rotation(const Vec3D &v, double rv);

    Matrix4x4 static View(const Matrix4x4 &transformMatrix);

    Matrix4x4 static Projection(double fov = 90.0, double aspect = 1.0, double ZNear = 1.0, double ZFar = 10.0);

    Matrix4x4 static ScreenSpace(int width, int height);
};


#endif //INC_3DZAVR_MATRIX4X4_H

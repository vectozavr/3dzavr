#ifndef LINALG_MATRIX3X3_H
#define LINALG_MATRIX3X3_H

#include <array>
#include <iostream>

#include <linalg/Vec3D.h>

class Matrix3x3 final {
private:
    std::array<std::array<double, 3>, 3> _arr{};

public:
    Matrix3x3() = default;
    explicit Matrix3x3(const std::array<std::array<double, 3>, 3>& matrix) : _arr(matrix) {};
    Matrix3x3(const Vec3D& column1, const Vec3D& column2, const Vec3D& column3);

    Matrix3x3 &operator=(const Matrix3x3 &matrix3X3) = default;

    Matrix3x3 &operator+=(const Matrix3x3 &matrix3x3);

    [[nodiscard]] Matrix3x3 operator*(const Matrix3x3 &matrix3x3) const;
    [[nodiscard]] Matrix3x3 operator+(const Matrix3x3 &matrix3x3) const;
    [[nodiscard]] Matrix3x3 operator-(const Matrix3x3 &matrix3x3) const;

    [[nodiscard]] Matrix3x3 operator*(double number) const;

    [[nodiscard]] Vec3D operator*(const Vec3D &vec3D) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& matrix3X3);

    [[nodiscard]] Vec3D x() const;
    [[nodiscard]] Vec3D y() const;
    [[nodiscard]] Vec3D z() const;

    [[nodiscard]] Matrix3x3 inverse() const;

    [[nodiscard]] double abs() const;

    [[nodiscard]] Matrix3x3 static Identity();
    [[nodiscard]] Matrix3x3 static Zero();
    [[nodiscard]] Matrix3x3 static Constant(double value);
    [[nodiscard]] Matrix3x3 static Outer(const Vec3D &vec1, const Vec3D &vec2); // The outer product

};

#include "Matrix3x3.inl"

#endif //LINALG_MATRIX3X3_H

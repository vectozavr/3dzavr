#ifndef LINALG_MATRIX3X3_H
#define LINALG_MATRIX3X3_H

#include <array>
#include <iostream>

#include <linalg/Vec3D.h>

/**
 * @class Matrix3x3
 * @brief Represents a 3x3 matrix for transformations in 2D space.
 *
 * This class encapsulates operations and transformations that can be performed on a 3x3 matrix. It is primarily
 * used in graphics programming for operations such as translation, rotation, and scaling in 2D space.
 *
 * @private_section
 * _arr A 3x3 array of doubles representing the matrix elements.
 *
 * @public_section
 * Matrix3x3() Default constructor initializes the matrix to an identity matrix if no parameters are passed.
 * explicit Matrix3x3(const std::array<std::array<double, 3>, 3>& matrix) Initializes the matrix with a given 3x3 array.
 * Matrix3x3(const Vec3D& column1, const Vec3D& column2, const Vec3D& column3) Initializes the matrix with three column vectors.
 *
 * Matrix3x3 &operator=(const Matrix3x3 &matrix3X3) Default assignment operator.
 *
 * Matrix3x3 &operator+=(const Matrix3x3 &matrix3x3) Adds another matrix to this matrix.
 *
 * [[nodiscard]] Matrix3x3 operator*(const Matrix3x3 &matrix3x3) const Multiplies this matrix by another matrix.
 * [[nodiscard]] Matrix3x3 operator+(const Matrix3x3 &matrix3x3) const Adds another matrix to this matrix.
 * [[nodiscard]] Matrix3x3 operator-(const Matrix3x3 &matrix3x3) const Subtracts another matrix from this matrix.
 *
 * [[nodiscard]] Matrix3x3 operator*(double number) const Multiplies the matrix by a scalar.
 *
 * [[nodiscard]] Vec3D operator*(const Vec3D &vec3D) const Multiplies the matrix by a vector.
 *
 * friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& matrix3X3) Overloads the << operator to print the matrix elements.
 *
 * [[nodiscard]] Vec3D x() const Extracts the x-axis vector from the matrix.
 * [[nodiscard]] Vec3D y() const Extracts the y-axis vector from the matrix.
 * [[nodiscard]] Vec3D z() const Extracts the z-axis vector from the matrix.
 *
 * [[nodiscard]] Matrix3x3 inverse() const Computes the inverse of the matrix.
 *
 * [[nodiscard]] double abs() const Calculates the determinant of the matrix.
 */
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

    /**
     * @brief Computes the inverse of the matrix.
     *
     * This method calculates the inverse of the current 3x3 matrix. The inverse of a matrix is a crucial operation in linear algebra,
     * allowing for the solution of systems of linear equations, among other applications. The method ensures that the operation is only
     * performed on matrices where an inverse exists (i.e., the matrix is non-singular and its determinant is not zero).
     *
     * Note: The computation of the inverse is based on the determinant and adjugate of the matrix, and it may be computationally intensive
     * for large matrices or matrices close to singularity.
     *
     * @return Matrix3x3 The inverse of the current matrix. If the matrix is singular (determinant is zero), the behavior is undefined.
     */
    [[nodiscard]] Matrix3x3 inverse() const;

    [[nodiscard]] double abs() const;

    /**
     * @brief Creates an identity matrix.
     *
     * This static method generates a 3x3 identity matrix, where all the elements on the main diagonal are 1, and all other elements are 0.
     * An identity matrix is a key element in matrix operations, serving as the neutral element for matrix multiplication.
     *
     * @return Matrix3x3 An identity matrix.
     */
    [[nodiscard]] Matrix3x3 static Identity();

    /**
     * @brief Creates a zero matrix.
     *
     * This static method generates a 3x3 zero matrix, where all elements are 0. A zero matrix is often used as the initial value in various matrix operations.
     *
     * @return Matrix3x3 A zero matrix.
     */
    [[nodiscard]] Matrix3x3 static Zero();

    /**
     * @brief Creates a constant matrix.
     *
     * This static method generates a 3x3 matrix where all elements are set to the specified value. This can be useful for various mathematical operations or initializations.
     *
     * @param value The value to set for all elements of the matrix.
     * @return Matrix3x3 A constant matrix with all elements set to the specified value.
     */
    [[nodiscard]] Matrix3x3 static Constant(double value);

    /**
     * @brief Creates a matrix from the outer product of two vectors.
     *
     * This static method calculates the outer product of two given vectors and creates a 3x3 matrix from the result. The outer product is a key operation in vector and matrix algebra, providing a way to generate matrices from vectors.
     *
     * @param vec1 The first vector for the outer product operation.
     * @param vec2 The second vector for the outer product operation.
     * @return Matrix3x3 A matrix generated from the outer product of the two vectors.
     */
    [[nodiscard]] Matrix3x3 static Outer(const Vec3D &vec1, const Vec3D &vec2);
};

#include "Matrix3x3.inl"

#endif //LINALG_MATRIX3X3_H

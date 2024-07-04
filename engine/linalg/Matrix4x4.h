#ifndef LINALG_MATRIX4X4_H
#define LINALG_MATRIX4X4_H

#include <array>
#include <iostream>

#include <linalg/Vec3D.h>

/**
 * @class Matrix4x4
 * @brief Represents a 4x4 matrix for transformations in 3D space.
 *
 * This class encapsulates operations and transformations that can be performed on a 4x4 matrix. It is primarily
 * used in graphics programming for operations such as translation, rotation, and scaling in 3D space.
 *
 * @private_section
 * _arr A 4x4 array of doubles representing the matrix elements.
 *
 * @private_method
 * arePerpendicularAxis() Checks if the axis of the matrix are perpendicular to each other.
 *
 * @public_section
 * Matrix4x4() Default constructor initializes the matrix to an identity matrix.
 * Matrix4x4(const std::array<std::array<double, 4>, 4>& matrix) Initializes the matrix with a given 4x4 array.
 *
 * operator=() Assignment operator for matrix copying.
 *
 * operator*() Multiplication operator for matrix-matrix multiplication, matrix-vector multiplication, and matrix-scalar multiplication.
 * operator+() Addition operator for matrix addition.
 * operator-() Subtraction operator for matrix subtraction.
 *
 * friend operator<< Overloads the << operator to print the matrix elements to an output stream.
 *
 * x(), y(), z(), w() Extracts the x, y, z, and w axis vectors from the matrix.
 *
 * inverse() Computes the inverse of the matrix. Note: computationally inefficient for certain applications.
 *
 * abs() Calculates the determinant of the matrix.
 */
class Matrix4x4 final {
private:
    std::array<std::array<double, 4>, 4> _arr{};

    [[nodiscard]] bool arePerpendicularAxis() const;
public:
    Matrix4x4() = default;
    explicit Matrix4x4(const std::array<std::array<double, 4>, 4>& matrix) : _arr(matrix) {};

    Matrix4x4 &operator=(const Matrix4x4 &matrix4X4) = default;

    [[nodiscard]] Matrix4x4 operator*(const Matrix4x4 &matrix4X4) const;
    [[nodiscard]] Matrix4x4 operator+(const Matrix4x4 &matrix4X4) const;
    [[nodiscard]] Matrix4x4 operator-(const Matrix4x4 &matrix4X4) const;

    [[nodiscard]] Vec4D operator*(const Vec4D &point4D) const;
    [[nodiscard]] Vec3D operator*(const Vec3D &vec) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix4x4);

    [[nodiscard]] Vec3D x() const;
    [[nodiscard]] Vec3D y() const;
    [[nodiscard]] Vec3D z() const;
    [[nodiscard]] Vec3D w() const;

    /**
     * @brief Computes the inverse of the matrix.
     *
     * This method calculates the inverse of the current 4x4 matrix. It is marked as computationally inefficient
     * due to the complexity of inverting a 4x4 matrix. For 3D applications that require transformations,
     * it is recommended to use the View() method instead, which relies on the inverse of a 3x3 matrix combined
     * with inverse translation, offering a more performance-optimized solution.
     *
     * Note: Use this method only when necessary, as it may impact performance significantly.
     *
     * @return Matrix4x4 The inverse of the current matrix.
     */
    [[nodiscard]] Matrix4x4 inverse() const;

    [[nodiscard]] double abs() const;

    /**
     * @brief Creates an identity matrix.
     *
     * An identity matrix is a square matrix with ones on the main diagonal and zeros elsewhere.
     * This matrix is used in matrix multiplication to leave a matrix unchanged.
     *
     * @return Matrix4x4 An identity matrix.
     */
    Matrix4x4 static Identity();

    /**
     * @brief Creates a zero matrix.
     *
     * A zero matrix is a matrix in which all elements are zero. It is used in various matrix operations
     * where a neutral element is required.
     *
     * @return Matrix4x4 A zero matrix.
     */
    Matrix4x4 static Zero();

    /**
     * @brief Creates a constant matrix.
     *
     * This matrix has all elements set to the specified value. It can be used in operations where a uniform
     * transformation is needed across all elements.
     *
     * @param value The value to set for all elements of the matrix.
     * @return Matrix4x4 A constant matrix.
     */
    Matrix4x4 static Constant(double value);

    /**
     * @brief Creates a scaling matrix.
     *
     * A scaling matrix is a matrix that scales an object by a factor along the x, y, and z axes.
     * It is used in 3D graphics to resize objects.
     *
     * @param factor The scaling factor along the x, y, and z axes.
     * @return Matrix4x4 A scaling matrix.
     */
    Matrix4x4 static Scale(const Vec3D &factor);

    /**
     * @brief Creates a translation matrix.
     *
     * A translation matrix is a matrix that moves an object by a specified distance along the x, y, and z axes.
     * It is used in 3D graphics to move objects around the scene.
     *
     * @param v The translation vector along the x, y, and z axes.
     * @return Matrix4x4 A translation matrix.
     */
    Matrix4x4 static Translation(const Vec3D &v);

    /**
     * @brief Creates a rotation matrix.
     *
     * A rotation matrix is a matrix that rotates an object by a specified angle around the x, y, and z axes.
     * It is used in 3D graphics to rotate objects in the scene.
     *
     * @param r The rotation vector around the x, y, and z axes.
     * @return Matrix4x4 A rotation matrix.
     */
    Matrix4x4 static Rotation(const Vec3D &r);

    /**
     * @brief Creates a rotation matrix around the x-axis.
     *
     * A rotation matrix around the x-axis is a matrix that rotates an object by a specified angle around the x-axis.
     * It is used in 3D graphics to rotate objects in the scene.
     *
     * @param rx The rotation angle around the x-axis.
     * @return Matrix4x4 A rotation matrix around the x-axis.
     */
    Matrix4x4 static RotationX(double rx);

    /**
     * @brief Creates a rotation matrix around the y-axis.
     *
     * A rotation matrix around the y-axis is a matrix that rotates an object by a specified angle around the y-axis.
     * It is used in 3D graphics to rotate objects in the scene.
     *
     * @param ry The rotation angle around the y-axis.
     * @return Matrix4x4 A rotation matrix around the y-axis.
     */
    Matrix4x4 static RotationY(double ry);

    /**
     * @brief Creates a rotation matrix around the z-axis.
     *
     * A rotation matrix around the z-axis is a matrix that rotates an object by a specified angle around the z-axis.
     * It is used in 3D graphics to rotate objects in the scene.
     *
     * @param rz The rotation angle around the z-axis.
     * @return Matrix4x4 A rotation matrix around the z-axis.
     */
    Matrix4x4 static RotationZ(double rz);

    /**
     * @brief Creates a rotation matrix around an arbitrary axis.
     *
     * A rotation matrix around an arbitrary axis is a matrix that rotates an object by a specified angle around an
     * arbitrary axis. It is used in 3D graphics to rotate objects in the scene.
     *
     * @param v The rotation axis.
     * @param rv The rotation angle around the axis.
     * @return Matrix4x4 A rotation matrix around an arbitrary axis.
     */
    Matrix4x4 static Rotation(const Vec3D &v, double rv);

    /**
     * @brief Creates a view matrix.
     *
     * A view matrix is a matrix that transforms objects from world space to camera space. It is used in 3D graphics
     * to position objects in the scene relative to the camera.
     *
     * @param transformMatrix The transformation matrix.
     * @return Matrix4x4 A view matrix.
     */
    Matrix4x4 static View(const Matrix4x4 &transformMatrix);

    /**
     * @brief Creates a projection matrix.
     *
     * A projection matrix is a matrix that transforms objects from camera space to screen space. It is used in 3D
     * graphics to project objects onto the screen.
     *
     * @param fov The field of view angle.
     * @param aspect The aspect ratio.
     * @param ZNear The near clipping plane.
     * @param ZFar The far clipping plane.
     * @return Matrix4x4 A projection matrix.
     */
    Matrix4x4 static Projection(double fov = 90.0, double aspect = 1.0, double ZNear = 1.0, double ZFar = 10.0);

    /**
     * @brief Creates a screen space matrix.
     *
     * A screen space matrix is a matrix that transforms objects from screen space to world space. It is used in 3D
     * graphics to position objects on the screen.
     *
     * @param width The width of the screen.
     * @param height The height of the screen.
     * @param shiftX The x-axis shift.
     * @param shiftY The y-axis shift.
     * @return Matrix4x4 A screen space matrix.
     */
    Matrix4x4 static ScreenSpace(uint16_t width, uint16_t height, int shiftX=0, int shiftY=0);
};

#include "Matrix4x4.inl"

#endif //LINALG_MATRIX4X4_H

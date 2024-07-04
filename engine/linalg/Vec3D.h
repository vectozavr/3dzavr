#ifndef LINALG_VEC3D_H
#define LINALG_VEC3D_H

#include <array>

#include <linalg/Vec4D.h>

/**
 * @class Vec3D
 * @brief Represents a three-dimensional vector.
 *
 * This class encapsulates a three-dimensional vector and provides functionality for vector arithmetic,
 * comparison, and geometric operations. It is designed for use in applications involving linear algebra,
 * physics calculations, and 3D graphics.
 *
 * @private_section
 * _arr_point An array of three doubles representing the vector components.
 *
 * @public_section
 * Vec3D() Default constructor initializes the vector to (0, 0, 0).
 * Vec3D(const Vec3D &vec) Copy constructor.
 * explicit Vec3D(const Vec4D &vec) Constructs a Vec3D from a Vec4D, ignoring the w component.
 * explicit Vec3D(double x, double y = 0.0, double z = 0.0) Constructs a vector with the given components.
 *
 * Vec3D &operator=(const Vec3D &) Default assignment operator.
 *
 * const double& x() const Returns the x component.
 * const double& y() const Returns the y component.
 * const double& z() const Returns the z component.
 *
 * double& operator[](std::size_t i) Returns a reference to the specified component (non-const version).
 * const double& operator[](std::size_t i) const Returns a reference to the specified component (const version).
 *
 * Vec3D operator-() const Returns the negation of the vector.
 *
 * bool operator==(const Vec3D &vec) const Compares two vectors for equality.
 * bool operator!=(const Vec3D &vec) const Compares two vectors for inequality.
 * bool operator<(const Vec3D &vec) const Compares two vectors to determine ordering.
 *
 * Vec3D &operator+=(const Vec3D &vec) Adds another vector to this vector.
 * Vec3D operator+(const Vec3D &vec) const Returns the sum of two vectors.
 *
 * Vec3D &operator-=(const Vec3D &vec) Subtracts another vector from this vector.
 * Vec3D operator-(const Vec3D &vec) const Returns the difference of two vectors.
 *
 * double dot(const Vec3D &vec) const Returns the dot product with another vector.
 * Vec3D cross(const Vec3D &vec) const Returns the cross product with another vector.
 *
 * Vec3D &operator*=(double number) Multiplies the vector by a scalar.
 * Vec3D operator*(double number) const Returns the product of the vector and a scalar.
 *
 * Vec3D &operator/=(double number) Divides the vector by a scalar.
 * Vec3D operator/(double number) const Returns the quotient of the vector and a scalar.
 *
 * double sqrAbs() const Returns the squared magnitude of the vector.
 * double abs() const Returns the magnitude of the vector.
 * Vec3D normalized() const Returns a normalized copy of the vector.
 * Vec4D makePoint4D() const Converts the Vec3D into a Vec4D with a w-component of 1.
 * Vec3D Random(); Returns a random vector with components in the range [0, 1).
 * Vec3D i(); Returns a unit vector in the x-direction.
 * Vec3D j(); Returns a unit vector in the y-direction.
 * Vec3D k(); Returns a unit vector in the z-direction.
 * Vec3D EPS(); Returns a vector with small components for use in floating-point comparisons.
 */

class Vec3D final {
private:
    std::array<double, 3> _arr_point{};

public:
    Vec3D() = default;
    Vec3D(const Vec3D &vec);
    explicit Vec3D(const Vec4D &vec);
    explicit Vec3D(double x, double y = 0.0, double z = 0.0);

    Vec3D &operator=(const Vec3D &) = default;

    [[nodiscard]] inline const double& x() const { return _arr_point[0]; }
    [[nodiscard]] inline const double& y() const { return _arr_point[1]; }
    [[nodiscard]] inline const double& z() const { return _arr_point[2]; }

    [[nodiscard]] inline double& operator[](std::size_t i) { return _arr_point[i]; }
    [[nodiscard]] inline const double& operator[](std::size_t i) const { return _arr_point[i]; }

    [[nodiscard]] Vec3D operator-() const;

    // Boolean operations
    bool operator==(const Vec3D &vec) const;
    bool operator!=(const Vec3D &vec) const;
    bool operator<(const Vec3D &vec) const;

    // Operations with Vec3D
    Vec3D &operator+=(const Vec3D &vec);
    [[nodiscard]] Vec3D operator+(const Vec3D &vec) const;

    Vec3D &operator-=(const Vec3D &vec);
    [[nodiscard]] Vec3D operator-(const Vec3D &vec) const;

    [[nodiscard]] double dot(const Vec3D &vec) const; // Returns dot product
    [[nodiscard]] Vec3D cross(const Vec3D &vec) const; // Returns cross product

    // Operations with numbers
    Vec3D &operator*=(double number);
    [[nodiscard]] Vec3D operator*(double number) const;

    Vec3D &operator/=(double number);
    [[nodiscard]] Vec3D operator/(double number) const;

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    [[nodiscard]] Vec3D normalized() const; // Returns normalized vector without changing
    [[nodiscard]] Vec4D makePoint4D() const;

    static Vec3D Random();
    static Vec3D i() { return Vec3D(1, 0, 0); }
    static Vec3D j() { return Vec3D(0, 1, 0); }
    static Vec3D k() { return Vec3D(0, 0, 1); }
    static Vec3D EPS() { return Vec3D(Consts::EPS, Consts::EPS, Consts::EPS); }

    /**
     * @brief Calculates the closest point and distance between two skew lines.
     *
     * Given two lines defined by points A, B (first line) and C, D (second line), this static method calculates the
     * closest point between these two skew lines and the distance between them. If the lines intersect, the distance
     * will be 0.
     *
     * @param A The starting point of the first line.
     * @param B The ending point of the first line.
     * @param C The starting point of the second line.
     * @param D The ending point of the second line.
     * @return std::pair<Vec3D, double> A pair containing the closest point between the two lines as a Vec3D and the
     * distance between them as a double.
     */
    static std::pair<Vec3D, double> intersectionOfLines(const Vec3D& A, const Vec3D& B, const Vec3D& C, const Vec3D& D);
};

#include "Vec3D.inl"

#endif //LINALG_VEC3D_H

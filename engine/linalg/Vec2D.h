#ifndef LINALG_VEC2D_H
#define LINALG_VEC2D_H

#include <array>

#include <linalg/Vec4D.h>

/**
 * @class Vec2D
 * @brief Represents a two-dimensional vector.
 *
 * This class encapsulates a two-dimensional vector and provides basic operations such as vector arithmetic,
 * comparison, and normalization. It is designed for use in linear algebra, graphics, and physics calculations.
 *
 * @private_section
 * _arr_point An array of two doubles representing the vector components.
 *
 * @public_section
 * Vec2D() Default constructor initializes the vector to (0, 0).
 * Vec2D(const Vec2D &vec) Copy constructor.
 * explicit Vec2D(const Vec4D &vec) Constructs a Vec2D from a Vec4D, taking its x and y components.
 * explicit Vec2D(double x, double y = 0.0) Constructs a vector with the given x and y components.
 *
 * Vec2D &operator=(const Vec2D &) = default Assignment operator.
 *
 * const double& x() const Returns the x component.
 * const double& y() const Returns the y component.
 *
 * double& operator[](std::size_t i) Returns a reference to the specified component (non-const version).
 * const double& operator[](std::size_t i) const Returns a reference to the specified component (const version).
 *
 * Vec2D operator-() const Returns the negation of the vector.
 *
 * bool operator==(const Vec2D &vec) const Compares two vectors for equality.
 * bool operator!=(const Vec2D &vec) const Compares two vectors for inequality.
 *
 * Vec2D &operator+=(const Vec2D &vec) Adds another vector to this vector.
 * Vec2D operator+(const Vec2D &vec) const Returns the sum of two vectors.
 *
 * Vec2D &operator-=(const Vec2D &vec) Subtracts another vector from this vector.
 * Vec2D operator-(const Vec2D &vec) const Returns the difference of two vectors.
 *
 * double dot(const Vec2D &vec) const Returns the dot product of two vectors.
 *
 * Vec2D &operator/=(double number) Divides the vector by a scalar.
 * Vec2D operator/(double number) const Returns the quotient of the vector and a scalar.
 *
 * Vec2D &operator*=(double number) Multiplies the vector by a scalar.
 * Vec2D operator*(double number) const Returns the product of the vector and a scalar.
 *
 * double sqrAbs() const Returns the squared magnitude of the vector.
 * double abs() const Returns the magnitude of the vector.
 * Vec2D normalized() const Returns a normalized copy of the vector.
 */
class Vec2D final {
private:
    std::array<double, 2> _arr_point{};

public:
    Vec2D() = default;
    Vec2D(const Vec2D &vec);
    explicit Vec2D(const Vec4D &vec);
    explicit Vec2D(double x, double y = 0.0);

    Vec2D &operator=(const Vec2D &) = default;

    [[nodiscard]] inline const double& x() const { return _arr_point[0]; }
    [[nodiscard]] inline const double& y() const { return _arr_point[1]; }

    [[nodiscard]] inline double& operator[](std::size_t i) { return _arr_point[i]; }
    [[nodiscard]] inline const double& operator[](std::size_t i) const { return _arr_point[i]; }

    [[nodiscard]] Vec2D operator-() const;

    // Boolean operations
    bool operator==(const Vec2D &vec) const;
    bool operator!=(const Vec2D &vec) const;

    // Operations with Vec2D
    Vec2D &operator+=(const Vec2D &vec);
    [[nodiscard]] Vec2D operator+(const Vec2D &vec) const;

    Vec2D &operator-=(const Vec2D &vec);
    [[nodiscard]] Vec2D operator-(const Vec2D &vec) const;

    [[nodiscard]] double dot(const Vec2D &vec) const; // Returns dot product

    // Operations with numbers
    Vec2D &operator/=(double number);
    [[nodiscard]] Vec2D operator/(double number) const;

    Vec2D &operator*=(double number);
    [[nodiscard]] Vec2D operator*(double number) const;

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    [[nodiscard]] Vec2D normalized() const; // Returns normalized vector without changing
};

#include "Vec2D.inl"

#endif //LINALG_VEC2D_H

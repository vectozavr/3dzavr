#ifndef LINALG_VEC4D_H
#define LINALG_VEC4D_H

#include <array>

/**
 * @class Vec4D
 * @brief Represents a four-dimensional vector.
 *
 * This class encapsulates a four-dimensional vector and provides basic operations such as vector arithmetic,
 * comparison, and normalization. It is designed for use in linear algebra, graphics, and physics calculations.
 *
 * @private_section
 * _arr_point An array of four doubles representing the vector components.
 *
 * @public_section
 * Vec4D() Default constructor initializes the vector to (0, 0, 0, 0).
 * Vec4D(const Vec4D &vec) Copy constructor.
 * explicit Vec4D(double x, double y = 0.0, double z = 0.0, double w = 0.0) Constructs a vector with the given components.
 *
 * Vec4D &operator=(const Vec4D &) Default assignment operator.
 *
 * const double& x() const Returns the x component.
 * const double& y() const Returns the y component.
 * const double& z() const Returns the z component.
 * const double& w() const Returns the w component.
 *
 * double& operator[](std::size_t i) Returns a reference to the specified component (non-const version).
 * const double& operator[](std::size_t i) const Returns a reference to the specified component (const version).
 *
 * Vec4D operator-() const Returns the negation of the vector.
 *
 * bool operator==(const Vec4D &vec) const Compares two vectors for equality.
 * bool operator!=(const Vec4D &vec) const Compares two vectors for inequality.
 *
 * Vec4D &operator+=(const Vec4D &vec) Adds another vector to this vector.
 * Vec4D operator+(const Vec4D &vec) const Returns the sum of two vectors.
 *
 * Vec4D &operator-=(const Vec4D &vec) Subtracts another vector from this vector.
 * Vec4D operator-(const Vec4D &vec) const Returns the difference of two vectors.
 *
 * Vec4D &operator*=(double number) Multiplies the vector by a scalar.
 * Vec4D operator*(double number) const Returns the product of the vector and a scalar.
 *
 * Vec4D &operator/=(double number) Divides the vector by a scalar.
 * Vec4D operator/(double number) const Returns the quotient of the vector and a scalar.
 *
 * double sqrAbs() const Returns the squared magnitude of the vector.
 * double abs() const Returns the magnitude of the vector.
 * Vec4D normalized() const Returns a normalized copy of the vector.
 */
class Vec4D final {
private:
    std::array<double, 4> _arr_point{};

public:
    Vec4D() = default;
    Vec4D(const Vec4D &vec);
    explicit Vec4D(double x, double y = 0.0, double z = 0.0, double w = 0.0);

    Vec4D &operator=(const Vec4D &) = default;

    [[nodiscard]] inline const double& x() const { return _arr_point[0]; }
    [[nodiscard]] inline const double& y() const { return _arr_point[1]; }
    [[nodiscard]] inline const double& z() const { return _arr_point[2]; }
    [[nodiscard]] inline const double& w() const { return _arr_point[3]; }

    [[nodiscard]] inline double& operator[](std::size_t i) { return _arr_point[i]; }
    [[nodiscard]] inline const double& operator[](std::size_t i) const { return _arr_point[i]; }

    [[nodiscard]] Vec4D operator-() const;

    // Boolean operations
    bool operator==(const Vec4D &vec) const;
    bool operator!=(const Vec4D &vec) const;

    // Operations with Vec4D
    Vec4D &operator+=(const Vec4D &vec);
    [[nodiscard]] Vec4D operator+(const Vec4D &vec) const;

    Vec4D &operator-=(const Vec4D &vec);
    [[nodiscard]] Vec4D operator-(const Vec4D &vec) const;

    // Operations with numbers
    Vec4D &operator*=(double number);
    [[nodiscard]] Vec4D operator*(double number) const;

    Vec4D &operator/=(double number);
    [[nodiscard]] Vec4D operator/(double number) const;

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    [[nodiscard]] Vec4D normalized() const; // Returns normalized vector without changing
};

#include "Vec4D.inl"

#endif //LINALG_VEC4D_H

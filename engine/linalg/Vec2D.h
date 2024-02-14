//
// Created by Иван Ильин on 10.10.2021.
//

#ifndef SHOOTER_VEC2D_H
#define SHOOTER_VEC2D_H

#include <array>

#include <linalg/Vec4D.h>

class Vec2D final {
private:
    std::array<double, 2> _arr_point{};

public:
    Vec2D() = default;
    Vec2D(const Vec2D &vec);
    explicit Vec2D(const Vec4D &point4D);
    explicit Vec2D(double x, double y = 0.0);

    Vec2D &operator=(const Vec2D &) = default;

    [[nodiscard]] inline double x() const { return _arr_point[0]; }
    [[nodiscard]] inline double y() const { return _arr_point[1]; }

    [[nodiscard]] Vec2D operator-() const &;
    [[nodiscard]] Vec2D &operator-() &&;

    // Boolean operations
    bool operator==(const Vec2D &vec) const;
    bool operator!=(const Vec2D &vec) const;

    // Operations with Vec2D
    Vec2D &operator+=(const Vec2D &vec);
    [[nodiscard]] Vec2D operator+(const Vec2D &vec) const &;
    [[nodiscard]] Vec2D &operator+(const Vec2D &vec) &&;

    Vec2D &operator-=(const Vec2D &vec);
    [[nodiscard]] Vec2D operator-(const Vec2D &vec) const &;
    [[nodiscard]] Vec2D &operator-(const Vec2D &vec) &&;

    [[nodiscard]] double dot(const Vec2D &vec) const; // Returns dot product

    // Operations with numbers
    Vec2D &operator/=(double number);
    [[nodiscard]] Vec2D operator/(double number) const &;
    [[nodiscard]] Vec2D &operator/(double number) &&;

    Vec2D &operator*=(double number);
    [[nodiscard]] Vec2D operator*(double number) const &;
    [[nodiscard]] Vec2D &operator*(double number) &&;

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    [[nodiscard]] Vec2D normalized() const; // Returns normalized vector without changing
};


#endif //SHOOTER_VEC2D_H

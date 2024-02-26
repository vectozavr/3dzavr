//
// Created by Иван Ильин on 12.01.2021.
//

#ifndef ENGINE_VEC4D_H
#define ENGINE_VEC4D_H

#include <array>

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

    [[nodiscard]] Vec4D operator-() const &;
    [[nodiscard]] Vec4D &operator-() &&;

    // Boolean operations
    bool operator==(const Vec4D &vec) const;
    bool operator!=(const Vec4D &vec) const;

    // Operations with Vec4D
    Vec4D &operator+=(const Vec4D &vec);
    [[nodiscard]] Vec4D operator+(const Vec4D &vec) const &;
    [[nodiscard]] Vec4D &operator+(const Vec4D &vec) &&;

    Vec4D &operator-=(const Vec4D &vec);
    [[nodiscard]] Vec4D operator-(const Vec4D &vec) const &;
    [[nodiscard]] Vec4D &operator-(const Vec4D &vec) &&;

    // Operations with numbers
    Vec4D &operator*=(double number);
    [[nodiscard]] Vec4D operator*(double number) const &;
    [[nodiscard]] Vec4D &operator*(double number) &&;

    Vec4D &operator/=(double number);
    [[nodiscard]] Vec4D operator/(double number) const &;
    [[nodiscard]] Vec4D &operator/(double number) &&;

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    [[nodiscard]] Vec4D normalized() const; // Returns normalized vector without changing
};

#endif //ENGINE_VEC4D_H

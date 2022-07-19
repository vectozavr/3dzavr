//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_TRIANGLE_H
#define ENGINE_TRIANGLE_H

#include <SFML/Graphics.hpp>

#include "math/Vec4D.h"
#include "math/Vec3D.h"
#include "math/Matrix4x4.h"

class Triangle final {
private:
    sf::Color _color;
    Vec4D _points[3];
    Vec3D _normal;

    void calculateNormal();
public:
    Triangle() = default;

    Triangle(const Triangle &triangle);

    Triangle(const Vec4D &p1, const Vec4D &p2, const Vec4D &p3, sf::Color color = {0, 0, 0});

    Triangle &operator=(const Triangle &) = default;

    [[nodiscard]] const Vec4D& operator[](int i) const;

    [[nodiscard]] Vec3D position() const { return Vec3D(_points[0] + _points[1] + _points[2])/3; }

    [[nodiscard]] Vec3D norm() const;

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4 &matrix4X4) const;

    [[nodiscard]] bool isPointInside(const Vec3D &point) const;

    [[nodiscard]] sf::Color color() const { return _color; }

    void setColor(sf::Color newColor) { _color = newColor; }

    [[nodiscard]] double distance(const Vec3D &vec) const { return norm().dot(Vec3D(_points[0]) - vec); }
};


#endif //INC_3DZAVR_TRIANGLE_H

#ifndef GEOMETRY_TRIANGLE_H
#define GEOMETRY_TRIANGLE_H

#include <linalg/Vec4D.h>
#include <linalg/Vec3D.h>
#include <linalg/Vec2D.h>
#include "objects/props/Color.h"
#include <linalg/Matrix4x4.h>
#include <Consts.h>

class Triangle final {
private:
    std::array<Vec4D, 3> _points;
    /*
     * Texture coordinates are in 3D because we need to work with its 3th component
     * during de-homogenization procedure for correct perspective projection (see Screen::drawTriangle() for example)
     */
    std::array<Vec3D, 3> _textureCoordinates;

    Vec3D _normal;

    void calculateNormal();
public:
    Triangle() = default;

    Triangle(const Triangle &triangle) = default;

    Triangle(const std::array<Vec4D, 3>& p, const std::array<Vec3D, 3>& uv = {Vec3D{0, 0, 1},
                                                                              Vec3D{0, 0, 1},
                                                                              Vec3D{0, 0, 1}});

    Triangle &operator=(const Triangle &) = default;

    [[nodiscard]] inline const Vec4D& operator[](int i) const { return _points[i]; }

    [[nodiscard]] inline Vec3D position() const { return Vec3D(_points[0] + _points[1] + _points[2])/3; }

    [[nodiscard]] inline Vec3D norm() const { return _normal; }

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4 &matrix4X4) const;

    [[nodiscard]] bool isPointInside(const Vec3D &point) const;

    [[nodiscard]] inline const std::array<Vec4D, 3>& points() const {return _points; }
    [[nodiscard]] inline const std::array<Vec3D, 3>& textureCoordinates() const { return _textureCoordinates; }

    [[nodiscard]] double distance(const Vec3D &vec) const { return norm().dot(Vec3D(_points[0]) - vec); }

    [[nodiscard]] Vec3D abgBarycCoord(const Vec2D& point) const;
    [[nodiscard]] Vec2D uvCoord(const Vec2D& point) const;
    [[nodiscard]] Vec2D uvCoordDer(const Vec2D& point) const;
};


#endif //GEOMETRY_TRIANGLE_H

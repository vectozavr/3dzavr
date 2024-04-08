#ifndef GEOMETRY_LINE_H
#define GEOMETRY_LINE_H


#include "linalg/Vec4D.h"
#include "linalg/Vec3D.h"
#include "linalg/Vec2D.h"
#include "linalg/Matrix4x4.h"
#include "Consts.h"


class Line final {
private:
    Vec4D _p1 = Vec4D(0, 0, 0, 1);
    Vec4D _p2 = Vec4D(0, 0, 0, 1);
public:
    Line() = default;

    Line(const Line &line) = default;

    Line(const Vec4D& p1, const Vec4D& p2) : _p1(p1), _p2(p2) {};

    Line &operator=(const Line &) = default;
    [[nodiscard]] inline Vec3D center() const { return Vec3D(_p1 + _p2)/3; }

    // Operations with Matrix4x4
    [[nodiscard]] inline Line operator*(const Matrix4x4 &matrix4X4) const { return {matrix4X4*_p1, matrix4X4*_p2}; };

    [[nodiscard]] inline double distance(const Vec3D &vec) const { return (center() - vec).abs(); }

    [[nodiscard]] inline Vec4D p1() const { return _p1; }
    [[nodiscard]] inline Vec4D p2() const { return _p2; }
};

#endif //GEOMETRY_LINE_H

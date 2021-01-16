//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_TRIANGLE_H
#define INC_3DZAVR_TRIANGLE_H

#include "Point4D.h"
#include "Matrix4x4.h"

class Triangle {
public:
    Point4D p[3];

    Triangle () = default;
    Triangle (const Triangle& triangle) = default;
    Triangle (Point4D p1, Point4D p2, Point4D p3);
    Triangle& operator=(const Triangle& triangle) = default;

    [[nodiscard]] Point4D operator[] (int i) const;
    [[nodiscard]] Point4D& operator[] (int i);

    Point4D norm() const;

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4& matrix4X4) const;
    Triangle& operator*=(const Matrix4x4& matrix4X4);
};


#endif //INC_3DZAVR_TRIANGLE_H

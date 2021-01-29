//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_TRIANGLE_H
#define INC_3DZAVR_TRIANGLE_H

#include "utils/Point4D.h"
#include "utils/Matrix4x4.h"
#include <SFML/Graphics.hpp>

class Triangle {
public:
    // This is for clipping debug: you can distinguish how this triangle was clipped
    enum ClipMode {
        None = 0,
        Cropped,
        Doubled,
        Skipped
    };
    ClipMode clip = None;
    sf::Color color;
    Point4D p[3];

    Triangle ();
    Triangle (const Triangle& triangle);
    Triangle (const Point4D& p1, const Point4D& p2, const Point4D& p3);
    Triangle& operator=(const Triangle& triangle) = default;

    [[nodiscard]] Point4D operator[] (int i) const;
    [[nodiscard]] Point4D& operator[] (int i);

    [[nodiscard]] Point4D norm() const;

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4& matrix4X4) const;
    Triangle& operator*=(const Matrix4x4& matrix4X4);
    [[nodiscard]] Point4D pos() const;
};


#endif //INC_3DZAVR_TRIANGLE_H

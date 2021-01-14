//
// Created by Иван Ильин on 13.01.2021.
//

#include "Triangle.h"

Triangle::Triangle(Point4D p1, Point4D p2, Point4D p3) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}

Triangle Triangle::operator*(const Matrix4x4 &matrix4X4) const {
    return Triangle(*this) *= matrix4X4;
}

Triangle &Triangle::operator*=(const Matrix4x4 &matrix4X4) {
    p1 = matrix4X4 * p1;
    p2 = matrix4X4 * p2;
    p3 = matrix4X4 * p3;

    return *this;
}

Point4D Triangle::norm() {

    Point4D v1 = p2 - p1;
    Point4D v2 = p3 - p1;

    return v1.cross3D(v2);
}

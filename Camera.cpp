//
// Created by Иван Ильин on 14.01.2021.
//

#include "Camera.h"

std::vector<Triangle> &Camera::project(const std::vector<Triangle> &tris) {
    this->v_tris = std::vector<Triangle>(tris);
    Matrix4x4 P = Matrix4x4::Projection(fov, aspect, ZNear, ZFar);

    // TODO: implement multithreading
    for(auto& t : v_tris)
        t *= P;

    return this->v_tris;
}

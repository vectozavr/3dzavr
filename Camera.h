//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_CAMERA_H
#define INC_3DZAVR_CAMERA_H

#include <vector>
#include "Triangle.h"

class Camera {
private:
    Point4D position;

    double fov = 90.0;
    double ZNear = 1.0;
    double ZFar = 10.0;
    double aspect = 1.0;

    std::vector<Triangle> v_tris;
public:
    Camera() = default;

    std::vector<Triangle>& project(const std::vector<Triangle>& tris);
    std::vector<Triangle>& data() { return v_tris; }
};


#endif //INC_3DZAVR_CAMERA_H

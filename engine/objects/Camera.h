//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <vector>

#include "objects/geometry/Plane.h"
#include "objects/geometry/Mesh.h"

class Camera final : public Object {
private:
    std::vector<Plane> _clipPlanes{};
    bool _ready = false;
    double _aspect = 0;

    Matrix4x4 _SP;
public:
    Camera() : Object(ObjectTag("Camera")) {};

    Camera(const Camera &camera) = delete;

    void setup(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<std::pair<std::shared_ptr<Triangle>, std::shared_ptr<Material>>> project(std::shared_ptr<Mesh> mesh);

    ~Camera();
};


#endif //INC_3DZAVR_CAMERA_H

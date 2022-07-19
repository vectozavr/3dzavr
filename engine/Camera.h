//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <vector>

#include <SFML/OpenGL.hpp>

#include "math/Plane.h"
#include "Mesh.h"

class Camera final : public Object {
private:
    std::vector<std::shared_ptr<Triangle>> _triangles{};
    std::vector<Plane> _clipPlanes{};
    bool _ready = false;
    double _aspect = 0;

    Matrix4x4 _SP;
public:
    Camera() : Object(ObjectNameTag("Camera")) {};

    Camera(const Camera &camera) = delete;

    void init(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<std::shared_ptr<Triangle>> project(std::shared_ptr<Mesh> mesh);

    void clear();

    [[nodiscard]] int buffSize() const { return _triangles.size(); }

    std::vector<std::shared_ptr<Triangle>> sorted();

};


#endif //INC_3DZAVR_CAMERA_H

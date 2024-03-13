#ifndef OBJECTS_CAMERA_H
#define OBJECTS_CAMERA_H

#include <vector>

#include "objects/geometry/Plane.h"
#include "objects/geometry/Mesh.h"

class Camera final : public Object {
private:
    std::vector<Plane> _clipPlanes;
    double _znear = 0;
    double _zfar = 0;
    double _fov = 0;
    bool _ready = false;
    double _aspect = 0;
    // Internal variables to reduce allocations
    std::vector<std::pair<Vec3D, Vec3D>> _clipBuffer1;
    std::vector<std::pair<Vec3D, Vec3D>> _clipBuffer2;

    Matrix4x4 _SP;
public:
    Camera() : Object(ObjectTag("Camera")) {};

    Camera(const Camera &camera) = delete;

    void setup(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<std::pair<Triangle, Triangle>> project(const Mesh& mesh);
};


#endif //OBJECTS_CAMERA_H

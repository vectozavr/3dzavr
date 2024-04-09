#ifndef OBJECTS_CAMERA_H
#define OBJECTS_CAMERA_H

#include <vector>

#include <components/geometry/Plane.h>
#include <components/geometry/TriangleMesh.h>
#include <components/geometry/LineMesh.h>

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

    std::shared_ptr<TransformMatrix> _transformMatrix;
public:
    Camera() : Object(ObjectTag("Camera")) {
        _transformMatrix = addComponent<TransformMatrix>();
    };

    Camera(const Camera &camera) = delete;

    void init(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<std::pair<Triangle, Triangle>> project(const TriangleMesh& triangleMesh);
    std::vector<Line> project(const LineMesh& lineMesh);

    std::shared_ptr<TransformMatrix> transformMatrix() const { return _transformMatrix; }
};


#endif //OBJECTS_CAMERA_H

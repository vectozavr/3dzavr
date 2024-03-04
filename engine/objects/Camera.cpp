#include <cmath>

#include "utils/Log.h"
#include "Camera.h"
#include "Consts.h"

std::vector<Triangle> Camera::project(const Mesh& mesh) {

    std::vector<Triangle> result{};

    if (!_ready) {
        Log::log("Camera::project(): cannot project _tris without camera initialization ( Camera::setup() ) ");
        return result;
    }

    if (!mesh.isVisible()) {
        return result;
    }

    // Model transform matrix: translate _tris in the origin of body.
    Matrix4x4 objectToCamera = fullInvModel() * mesh.fullModel();

    // Transform mesh bounds into camera coordinates
    Vec3D center = Vec3D(objectToCamera * mesh.bounds().center.makePoint4D());
    Vec3D left = objectToCamera.x() * mesh.bounds().extents.x();
    Vec3D up = objectToCamera.y() * mesh.bounds().extents.y();
    Vec3D forward = objectToCamera.z() * mesh.bounds().extents.z();
    Vec3D extents(
        std::abs(left.x()) + std::abs(up.x()) + std::abs(forward.x()),
        std::abs(left.y()) + std::abs(up.y()) + std::abs(forward.y()),
        std::abs(left.z()) + std::abs(up.z()) + std::abs(forward.z())
    );

    // Check if object bounds is inside camera frustum
    for (auto &plane : _clipPlanes) {
        double r =
            extents.x() * std::abs(plane.normal.x()) +
            extents.y() * std::abs(plane.normal.y()) +
            extents.z() * std::abs(plane.normal.z());

        if (plane.distance(center) + r < 0)
            return result;
    }

    for (auto &t : mesh.triangles()) {

        Triangle MTriangle = t * objectToCamera;
        double dot = MTriangle.norm().dot(MTriangle.position());

        if (dot > 0) {
            continue;
        }

        // In the beginning we need to translate triangle from object local coordinate to world coordinates:
        // After that we apply clipping for all planes from _clipPlanes

        _clipBuffer2.emplace_back(Vec3D(MTriangle.points()[0]), MTriangle.textureCoordinates()[0]);
        _clipBuffer2.emplace_back(Vec3D(MTriangle.points()[1]), MTriangle.textureCoordinates()[1]);
        _clipBuffer2.emplace_back(Vec3D(MTriangle.points()[2]), MTriangle.textureCoordinates()[2]);
        for (auto &plane : _clipPlanes) {
            _clipBuffer1.swap(_clipBuffer2);
            _clipBuffer2.clear();
            plane.clip(_clipBuffer1, _clipBuffer2);
        }

        // It's time to project our clipped polygon from 3D -> 2D
        // and transform its coordinate to screen space (in pixels):
        for (auto &vertex : _clipBuffer2) {
            Vec4D tmp = _SP * vertex.first.makePoint4D();
            vertex.first = Vec3D(tmp) / tmp.w();
            vertex.second /= tmp.w();
        }

        // Finally, create triangle from sorted list of vertices
        for (size_t i = 2; i < _clipBuffer2.size(); i++) {
            result.emplace_back(
                std::array<Vec4D, 3>{
                    _clipBuffer2[0].first.makePoint4D(),
                    _clipBuffer2[i - 1].first.makePoint4D(),
                    _clipBuffer2[i].first.makePoint4D()
                },
                std::array<Vec3D, 3>{
                    _clipBuffer2[0].second,
                    _clipBuffer2[i - 1].second,
                    _clipBuffer2[i].second
                }
            );
        }

        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        _clipBuffer1.clear();
        _clipBuffer2.clear();
    }

    return result;
}

void Camera::setup(int width, int height, double fov, double ZNear, double ZFar) {
    // We need to init camera only after creation or changing width, height, fov, ZNear or ZFar.
    // Because here we calculate matrix that does not change during the motion of _objects or camera
    _znear = ZNear;
    _zfar = ZFar;
    _fov = fov;
    _aspect = (double) width / (double) height;
    Matrix4x4 P = Matrix4x4::Projection(fov, _aspect, ZNear, ZFar);
    Matrix4x4 S = Matrix4x4::ScreenSpace(width, height);

    _SP = S * P; // screen-space-projections matrix

    // This is planes for clipping _tris.
    // Motivation: we are not interested in _tris that we cannot see.
    double thetta1 = Consts::PI * fov * 0.5 / 180.0;
    double thetta2 = atan(_aspect * tan(thetta1));
    _clipPlanes.reserve(6);
    _clipPlanes.emplace_back(Vec3D{0, 0, 1}, Vec3D{0, 0, ZNear}); // near plane
    _clipPlanes.emplace_back(Vec3D{0, 0, -1}, Vec3D{0, 0, ZFar}); // far plane
    _clipPlanes.emplace_back(Vec3D{-cos(thetta2), 0, sin(thetta2)}, -Consts::EPS); // left plane
    _clipPlanes.emplace_back(Vec3D{cos(thetta2), 0, sin(thetta2)}, -Consts::EPS); // right plane
    _clipPlanes.emplace_back(Vec3D{0, cos(thetta1), sin(thetta1)}, -Consts::EPS); // down plane
    _clipPlanes.emplace_back(Vec3D{0, -cos(thetta1), sin(thetta1)}, -Consts::EPS); // up plane

    // 3 vertices from triangle, 1 vertex from each plane clip
    _clipBuffer1.reserve(9);
    _clipBuffer2.reserve(9);

    _ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

//
// Created by Иван Ильин on 14.01.2021.
//

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
    Matrix4x4 objectToWorld = mesh.fullModel();
    Matrix4x4 worldToScreen = _SP * fullInvModel();

    // Transform mesh bounds into global coordinates
    Vec3D globalCenter = Vec3D(objectToWorld * mesh.bounds().center.makePoint4D());
    Vec3D left = mesh.left() * mesh.bounds().extents.x();
    Vec3D up = mesh.up() * mesh.bounds().extents.y();
    Vec3D forward = mesh.lookAt() * mesh.bounds().extents.z();

    double globalX = std::abs(left.x()) + std::abs(up.x()) + std::abs(forward.x());
    double globalY = std::abs(left.y()) + std::abs(up.y()) + std::abs(forward.y());
    double globalZ = std::abs(left.z()) + std::abs(up.z()) + std::abs(forward.z());

    // Check if object bounds is inside camera frustum
    for (auto &plane : _clipPlanes) {
        double r =
            globalX * std::abs(plane.normal.x()) +
            globalY * std::abs(plane.normal.y()) +
            globalZ * std::abs(plane.normal.z());

        if (plane.distance(globalCenter) + r < 0)
            return result;
    }

    for (auto &t : mesh.triangles()) {

        Triangle MTriangle = t * objectToWorld;

        double dot = MTriangle.norm().dot((Vec3D(MTriangle.position()) - position()).normalized());

        if (dot > 0) {
            continue;
        }

        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        _clipBuffer1.clear();
        _clipBuffer2.clear();

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
            Vec4D tmp = worldToScreen * vertex.first.makePoint4D();
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

    _clipPlanes.reserve(6);

    // 3 vertices from triangle, 1 vertex from each plane clip
    _clipBuffer1.reserve(9);
    _clipBuffer2.reserve(9);

    _ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

void Camera::updateFrustum() {
    _clipPlanes.clear();
    Matrix4x4 cameraModel = fullModel();

    Vec3D pos = this->position();
    Vec3D forward = this->lookAt();
    Vec3D left = this->left();
    Vec3D up = this->up();

    const double halfVSide = _zfar * tan(Consts::PI * _fov * 0.5 / 180.0);
    const double halfHSide = halfVSide * _aspect;
    const Vec3D frontMultFar = forward * _zfar;

    // This is planes for clipping _tris.
    // Motivation: we are not interested in _tris that we cannot see.
    _clipPlanes.emplace_back(forward, pos + forward * _znear);                  // near plane
    _clipPlanes.emplace_back(-forward, pos + frontMultFar);                     // far plane
    _clipPlanes.emplace_back((frontMultFar + left * halfHSide).cross(up), pos); // left plane
    _clipPlanes.emplace_back(up.cross(frontMultFar - left * halfHSide), pos);   // right plane
    _clipPlanes.emplace_back((frontMultFar - up * halfVSide).cross(left), pos); // bottom plane
    _clipPlanes.emplace_back(left.cross(frontMultFar + up * halfVSide), pos);   // top plane
    // Prevent transparent pixels at the edge of the screen
    for (size_t i = 2; i < _clipPlanes.size(); i++) {
        _clipPlanes[i].offset -= Consts::EPS;
    }
}

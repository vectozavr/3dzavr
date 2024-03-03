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

    for (auto &t : mesh.triangles()) {

        Triangle MTriangle = t * objectToWorld;

        double dot = MTriangle.norm().dot((Vec3D(MTriangle.position()) - position()).normalized());

        if (dot > 0) {
            continue;
        }

        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        _clippedTriangles.clear();
        _tempBuffer.clear();

        // In the beginning we need to translate triangle from object local coordinate to world coordinates:
        // After that we apply clipping for all planes from _clipPlanes
        _clippedTriangles.emplace_back(MTriangle);
        for (auto &plane : _clipPlanes) {
            while (!_clippedTriangles.empty()) {
                stack_vector<Triangle, 2> clipResult = plane.clip(_clippedTriangles.back());
                _clippedTriangles.pop_back();
                for (auto &i : clipResult) {
                    _tempBuffer.emplace_back(i);
                }
            }
            _clippedTriangles.swap(_tempBuffer);
        }

        for (auto &clipped : _clippedTriangles) {
            // Finally it's time to project our clipped colored drawTriangle from 3D -> 2D
            // and transform its coordinate to screen space (in pixels):
            Triangle clippedProjected = clipped * worldToScreen;
            auto clippedTexCoord = clippedProjected.textureCoordinates();

            result.emplace_back(
                std::array<Vec4D, 3>{
                    clippedProjected[0] / clippedProjected[0].w(),
                    clippedProjected[1] / clippedProjected[1].w(),
                    clippedProjected[2] / clippedProjected[2].w()
                },
                std::array<Vec3D, 3>{
                    clippedTexCoord[0] / clippedProjected[0].w(),
                    clippedTexCoord[1] / clippedProjected[1].w(),
                    clippedTexCoord[2] / clippedProjected[2].w()
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

    // The capacity is chosen simply by a small runtime test (though with proper clipping it should be 7 at most).
    _clippedTriangles.reserve(12);
    _tempBuffer.reserve(12);

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

#include <cmath>

#include <utils/Log.h>
#include <objects/Camera.h>
#include <Consts.h>

std::vector<std::pair<Triangle, Triangle>> Camera::project(const TriangleMesh& triangleMesh) {

    std::vector<std::pair<Triangle, Triangle>> result{};

    if (!_ready) {
        init(Consts::STANDARD_SCREEN_WIDTH, Consts::STANDARD_SCREEN_HEIGHT);
    }

    if (!triangleMesh.isVisible()) {
        return result;
    }
    // Model transform matrix: translate _tris in the origin of body.
    Matrix4x4 objectToCamera = _transformMatrix->fullInvModel() * triangleMesh.getComponent<TransformMatrix>()->fullModel();

    Matrix4x4 cameraToWorld = _transformMatrix->fullModel();

    // Transform mesh bounds into camera coordinates
    auto [center, extents] = triangleMesh.bounds()*objectToCamera;

    // Check if object bounds is inside camera frustum
    for (auto &plane : _clipPlanes) {
        double r =
            extents.x() * std::abs(plane.normal.x()) +
            extents.y() * std::abs(plane.normal.y()) +
            extents.z() * std::abs(plane.normal.z());

        if (plane.distance(center) + r < 0)
            return result;
    }

    for (auto &t : triangleMesh.triangles()) {

        // TODO: here we apply the same objectToCamera matrix every single frame. Might be improved by cashing.
        Triangle MTriangle = t * objectToCamera;
        double dot = MTriangle.norm().dot(Vec3D(MTriangle[0]));

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

        _clipBuffer1.clear();
        // It's time to project our clipped polygon from 3D -> 2D
        // and transform its coordinate to screen space (in pixels):
        for (auto &vertex : _clipBuffer2) {
            _clipBuffer1.emplace_back(vertex);

            Vec4D tmp = _SP * vertex.first.makePoint4D();
            vertex.first = Vec3D(tmp) / tmp.w();
            vertex.second /= tmp.w();
        }

        // Finally, create triangle from sorted list of vertices
        for (size_t i = 2; i < _clipBuffer2.size(); i++) {
            result.emplace_back(
                    // The first one is projected triangle
                    Triangle{std::array<Vec4D, 3>{
                            _clipBuffer2[0].first.makePoint4D(),
                            _clipBuffer2[i - 1].first.makePoint4D(),
                            _clipBuffer2[i].first.makePoint4D()
                        },
                             std::array<Vec3D, 3>{
                            _clipBuffer2[0].second,
                            _clipBuffer2[i - 1].second,
                            _clipBuffer2[i].second
                            }
                        },
                    // The second one is in the world space (not projected)
                        Triangle{std::array<Vec4D, 3>{
                            cameraToWorld*(_clipBuffer1[0].first.makePoint4D()),
                            cameraToWorld*(_clipBuffer1[i - 1].first.makePoint4D()),
                            cameraToWorld*(_clipBuffer1[i].first.makePoint4D())
                        },
                                 std::array<Vec3D, 3>{
                            _clipBuffer1[0].second,
                            _clipBuffer1[i - 1].second,
                            _clipBuffer1[i].second
                        }
                    }
            );
        }

        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        _clipBuffer1.clear();
        _clipBuffer2.clear();
    }

    return result;
}

void Camera::init(int width, int height, double fov, double ZNear, double ZFar) {
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

std::vector<Line> Camera::project(const LineMesh &lineMesh) {
    std::vector<Line> result{};

    if (!_ready) {
        Log::log("Camera::project(): cannot project lineMesh without camera initialization ( Camera::init(); ) ");
        return result;
    }

    if (!lineMesh.isVisible()) {
        return result;
    }
    // Model transform matrix: translate _tris in the origin of body.
    Matrix4x4 objectToCamera = _transformMatrix->fullInvModel() * lineMesh.getComponent<TransformMatrix>()->fullModel();

    // Transform mesh bounds into camera coordinates
    auto [center, extents] = lineMesh.bounds()*objectToCamera;

    // Check if object bounds is inside camera frustum
    for (auto &plane : _clipPlanes) {
        double r =
                extents.x() * std::abs(plane.normal.x()) +
                extents.y() * std::abs(plane.normal.y()) +
                extents.z() * std::abs(plane.normal.z());

        if (plane.distance(center) + r < 0)
            return result;
    }

    for (auto &line : lineMesh.lines()) {

        // TODO: here we apply the same objectToCamera matrix every single frame. Might be improved by cashing.

        Line MLine = line * objectToCamera;

        // In the beginning we need to translate lines from object local coordinate to world coordinates:
        // After that we apply clipping for all planes from _clipPlanes
        bool isFullyOutside = false;
        Line _clippedLine = MLine;
        for (auto &plane : _clipPlanes) {
            _clippedLine = plane.clip(_clippedLine, isFullyOutside);
            if(isFullyOutside) {
                break;
            }
        }
        if(isFullyOutside) {
            continue;
        }

        Line _projectedLine = _clippedLine*_SP;
        Line _projectedDehomLine = {_projectedLine.p1()/_projectedLine.p1().w(),
                                    _projectedLine.p2()/_projectedLine.p2().w() };

        result.emplace_back(_projectedDehomLine);
    }

    return result;
}

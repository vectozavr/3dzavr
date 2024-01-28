//
// Created by Иван Ильин on 14.01.2021.
//

#include <cmath>

#include "utils/Log.h"
#include "Camera.h"
#include "Consts.h"

std::vector<std::pair<std::shared_ptr<Triangle>, std::shared_ptr<Material>>> Camera::project(std::shared_ptr<Mesh> mesh) {

    std::vector<std::pair<std::shared_ptr<Triangle>, std::shared_ptr<Material>>> result{};

    if (!_ready) {
        Log::log("Camera::project(): cannot project _tris without camera initialization ( Camera::setup() ) ");
        return result;
    }

    if (!mesh->isVisible()) {
        return result;
    }

    // Model transform matrix: translate _tris in the origin of body.
    Matrix4x4 M = mesh->model();
    Matrix4x4 V = invModel();

    // We don't want to waste time re-allocating memory every time
    std::vector<Triangle> clippedTriangles, tempBuffer;

    for (auto &t : mesh->triangles()) {

        Triangle MTriangle = t * M;

        double dot = MTriangle.norm().dot((Vec3D(MTriangle.position()) - position()).normalized());

        if (dot > 0) {
            continue;
        }

        Triangle VMTriangle = MTriangle * V;

        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        clippedTriangles.clear();
        tempBuffer.clear();

        // In the beginning we need to translate triangle from world coordinate to our camera system:
        // After that we apply clipping for all planes from _clipPlanes
        clippedTriangles.emplace_back(VMTriangle);
        for (auto &plane : _clipPlanes) {
            while (!clippedTriangles.empty()) {
                std::vector<Triangle> clipResult = plane.clip(clippedTriangles.back());
                clippedTriangles.pop_back();
                for (auto &i : clipResult) {
                    tempBuffer.emplace_back(i);
                }
            }
            clippedTriangles.swap(tempBuffer);
        }

        for (auto &clipped : clippedTriangles) {
            // Finally its time to project our clipped colored drawTriangle from 3D -> 2D
            // and transform it's coordinate to screen space (in pixels):
            Triangle clippedProjected = clipped * _SP;
            auto clippedTexCoord = clippedProjected.textureCoordinates();

            Triangle clippedProjectedNormalized = Triangle({clippedProjected[0] / clippedProjected[0].w(),
                                                            clippedProjected[1] / clippedProjected[1].w(),
                                                            clippedProjected[2] / clippedProjected[2].w()},
                                                           {clippedTexCoord[0] / clippedProjected[0].w(),
                                                            clippedTexCoord[1] / clippedProjected[1].w(),
                                                            clippedTexCoord[2] / clippedProjected[2].w()});

            auto material = mesh->getMaterial();
            auto tri = std::make_shared<Triangle>(clippedProjectedNormalized);
            std::pair<std::shared_ptr<Triangle>, std::shared_ptr<Material>> pair(tri, material);

            result.emplace_back(pair);
        }
    }

    return result;
}

void Camera::setup(int width, int height, double fov, double ZNear, double ZFar) {
    // We need to init camera only after creation or changing width, height, fov, ZNear or ZFar.
    // Because here we calculate matrix that does not change during the motion of _objects or camera
    _aspect = (double) width / (double) height;
    Matrix4x4 P = Matrix4x4::Projection(fov, _aspect, ZNear, ZFar);
    Matrix4x4 S = Matrix4x4::ScreenSpace(width, height);

    _SP = S * P; // screen-space-projections matrix

    // This is planes for clipping _tris.
    // Motivation: we are not interested in _tris that we cannot see.
    _clipPlanes.emplace_back(Vec3D{0, 0, 1}, Vec3D{0, 0, ZNear}, ObjectTag("near")); // near plane
    _clipPlanes.emplace_back(Vec3D{0, 0, -1}, Vec3D{0, 0, ZFar}, ObjectTag("far")); // far plane

    double thetta1 = Consts::PI * fov * 0.5 / 180.0;
    double thetta2 = atan(_aspect * tan(thetta1));
    _clipPlanes.emplace_back(Vec3D{-cos(thetta2), 0, sin(thetta2)}, Vec3D{0, 0, 0}, ObjectTag("left")); // left plane
    _clipPlanes.emplace_back(Vec3D{cos(thetta2), 0, sin(thetta2)}, Vec3D{0, 0, 0}, ObjectTag("right")); // right plane
    _clipPlanes.emplace_back(Vec3D{0, cos(thetta1), sin(thetta1)}, Vec3D{0, 0, 0}, ObjectTag("down")); // down plane
    _clipPlanes.emplace_back(Vec3D{0, -cos(thetta1), sin(thetta1)}, Vec3D{0, 0, 0}, ObjectTag("up")); // up plane

    _ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

Camera::~Camera() {
    _clipPlanes.clear();
}

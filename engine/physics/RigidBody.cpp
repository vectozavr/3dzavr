//
// Created by Иван Ильин on 05.02.2021.
//

#include <cmath>
#include <utility>

#include "RigidBody.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include "../Consts.h"

RigidBody::RigidBody(ObjectNameTag nameTag, const std::string &filename, const Vec3D &scale, bool useSimpleBox) : Mesh(std::move(nameTag),
                                                                                                    filename, scale),
                                                                                               _hitBox(*this, useSimpleBox) {
}

RigidBody::RigidBody(const Mesh &mesh, bool useSimpleBox) : Mesh(mesh), _hitBox(mesh, useSimpleBox) {
}

Vec3D RigidBody::_findFurthestPoint(const Vec3D &direction) {
    Vec3D maxPoint{0, 0, 0};

    double maxDistance = -std::numeric_limits<double>::max();

    Vec3D transformedDirection = (invModel() * direction).normalized();

    for(auto & it : _hitBox) {
        double distance = it.dot(transformedDirection);

        if (distance > maxDistance) {
            maxDistance = distance;
            maxPoint = it;
        }
    }

    return model() * maxPoint + position();
}

Vec3D RigidBody::_support(std::shared_ptr<RigidBody> obj, const Vec3D &direction) {
    Vec3D p1 = _findFurthestPoint(direction);
    Vec3D p2 = obj->_findFurthestPoint(-direction);

    return p1 - p2;
}

NextSimplex RigidBody::_nextSimplex(const Simplex &points) {
    switch (points.type()) {
        case SimplexType::Line:
            return _lineCase(points);
        case SimplexType::Triangle:
            return _triangleCase(points);
        case SimplexType::Tetrahedron:
            return _tetrahedronCase(points);

        default:
            throw std::logic_error{"RigidBody::_nextSimplex: simplex is not Line, Triangle or Tetrahedron"};
    }
}

NextSimplex RigidBody::_lineCase(const Simplex &points) {
    Simplex newPoints(points);
    Vec3D newDirection;

    Vec3D a = points[0];
    Vec3D b = points[1];

    Vec3D ab = b - a;
    Vec3D ao = -a;

    if (ab.dot(ao) > 0) {
        newDirection = ab.cross(ao).cross(ab);
    } else {
        newPoints = Simplex{a};
        newDirection = ao;
    }

    return NextSimplex{newPoints, newDirection, false};
}

NextSimplex RigidBody::_triangleCase(const Simplex &points) {
    Simplex newPoints(points);
    Vec3D newDirection;

    Vec3D a = points[0];
    Vec3D b = points[1];
    Vec3D c = points[2];

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ao = -a;

    Vec3D abc = ab.cross(ac);

    if (abc.cross(ac).dot(ao) > 0) {
        if (ac.dot(ao) > 0) {
            newPoints = Simplex{a, c};
            newDirection = ac.cross(ao).cross(ac);
        } else {
            return _lineCase(Simplex{a, b});
        }
    } else {
        if (ab.cross(abc).dot(ao) > 0) {
            return _lineCase(Simplex{a, b});
        } else {
            if (abc.dot(ao) > 0) {
                newDirection = abc;
            } else {
                newPoints = Simplex{a, c, b};
                newDirection = -abc;
            }
        }
    }

    return NextSimplex{newPoints, newDirection, false};
}

NextSimplex RigidBody::_tetrahedronCase(const Simplex &points) {
    Vec3D a = points[0];
    Vec3D b = points[1];
    Vec3D c = points[2];
    Vec3D d = points[3];

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ad = d - a;
    Vec3D ao = -a;

    Vec3D abc = ab.cross(ac);
    Vec3D acd = ac.cross(ad);
    Vec3D adb = ad.cross(ab);

    if (abc.dot(ao) > 0) {
        return _triangleCase(Simplex{a, b, c});
    }

    if (acd.dot(ao) > 0) {
        return _triangleCase(Simplex{a, c, d});
    }

    if (adb.dot(ao) > 0) {
        return _triangleCase(Simplex{a, d, b});
    }

    return NextSimplex{points, Vec3D(), true};
}

std::pair<bool, Simplex> RigidBody::checkGJKCollision(std::shared_ptr<RigidBody> obj) {
    // This is implementation of GJK algorithm for collision detection.
    // It builds a simplex (a simplest shape that can select point in space) around
    // zero for Minkowski Difference. Collision happend when zero point is inside.
    // See references:
    // https://www.youtube.com/watch?v=MDusDn8oTSE
    // https://blog.winter.dev/2020/gjk-algorithm/


    // Get initial support point in any direction
    Vec3D support = _support(obj, Vec3D{1, 0, 0});

    // Simplex is an array of points, max count is 4
    Simplex points{};
    points.push_front(support);

    // New direction is towards the origin
    Vec3D direction = -support;

    size_t iters = 0;
    while (iters++ < size() + obj->size()) {
        support = _support(obj, direction);

        if (support.dot(direction) <= 0) {
            return std::make_pair(false, points); // no collision
        }

        points.push_front(support);

        NextSimplex nextSimplex = _nextSimplex(points);

        direction = nextSimplex.newDirection;
        points = nextSimplex.newSimplex;

        if (nextSimplex.finishSearching) {
            if (obj->isCollider()) {
                _inCollision = true;
            }
            return std::make_pair(true, points);
        }
    }
    return std::make_pair(false, points);
}

CollisionPoint RigidBody::EPA(const Simplex &simplex, std::shared_ptr<RigidBody> obj) {
    // This is implementation of EPA algorithm for solving collision.
    // It uses a simplex from GJK around and expand it to the border.
    // The goal is to calculate the nearest normal and the intersection depth.
    // See references:
    // https://www.youtube.com/watch?v=0XQ2FSz3EK8
    // https://blog.winter.dev/2020/epa-algorithm/

    std::vector<Vec3D> polytope(simplex.begin(), simplex.end());
    std::vector<size_t> faces = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2
    };

    auto faceNormals = _getFaceNormals(polytope, faces);
    std::vector<FaceNormal> normals = faceNormals.first;
    size_t minFace = faceNormals.second;

    Vec3D minNormal = normals[minFace].normal;
    double minDistance = std::numeric_limits<double>::max();

    size_t iters = 0;
    while (minDistance == std::numeric_limits<double>::max() && iters++ < size() + obj->size()) {
        minNormal = normals[minFace].normal;
        minDistance = normals[minFace].distance;

        Vec3D support = _support(obj, minNormal);
        double sDistance = minNormal.dot(support);

        if (std::abs(sDistance - minDistance) > Consts::EPA_EPS) {
            minDistance = std::numeric_limits<double>::max();
            std::vector<std::pair<size_t, size_t>> uniqueEdges;

            size_t f = 0;
            for (auto &normal : normals) {
                if (normal.normal.dot(support) > 0) {
                    uniqueEdges = _addIfUniqueEdge(uniqueEdges, faces, f + 0, f + 1);
                    uniqueEdges = _addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                    uniqueEdges = _addIfUniqueEdge(uniqueEdges, faces, f + 2, f + 0);

                    faces.erase(faces.begin() + f);
                    faces.erase(faces.begin() + f);
                    faces.erase(faces.begin() + f);
                } else {
                    f += 3;
                }
            }

            std::vector<size_t> newFaces;
            newFaces.reserve(uniqueEdges.size() * 3);
            for (auto[edgeIndex1, edgeIndex2] : uniqueEdges) {
                newFaces.push_back(edgeIndex1);
                newFaces.push_back(edgeIndex2);
                newFaces.push_back(polytope.size());
            }
            polytope.push_back(support);

            faces.insert(faces.end(), newFaces.begin(), newFaces.end());

            auto newFaceNormals = _getFaceNormals(polytope, faces);

            normals = std::move(newFaceNormals.first);
            minFace = newFaceNormals.second;
        }
    }

    _collisionNormal = minNormal;
    if (std::abs(minDistance - std::numeric_limits<double>::max()) < Consts::EPS) {
        return CollisionPoint{minNormal, 0};
    }

    return CollisionPoint{minNormal, minDistance + Consts::EPA_EPS};
}

std::pair<std::vector<FaceNormal>, size_t>
RigidBody::_getFaceNormals(const std::vector<Vec3D> &polytope, const std::vector<size_t> &faces) {
    std::vector<FaceNormal> normals;
    normals.reserve(faces.size() / 3);
    size_t nearestFaceIndex = 0;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < faces.size(); i += 3) {
        Vec3D a = polytope[faces[i + 0]];
        Vec3D b = polytope[faces[i + 1]];
        Vec3D c = polytope[faces[i + 2]];

        Vec3D normal = (b - a).cross(c - a).normalized();

        double distance = normal.dot(a);

        if (distance < -Consts::EPS) {
            normal = -normal;
            distance *= -1;
        }

        normals.emplace_back(FaceNormal{normal, distance});

        if (distance < minDistance) {
            nearestFaceIndex = i / 3;
            minDistance = distance;
        }
    }

    return {normals, nearestFaceIndex};
}

std::vector<std::pair<size_t, size_t>>
RigidBody::_addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces,
                            size_t a, size_t b) {

    std::vector<std::pair<size_t, size_t>> newEdges = edges;

    // We are interested in reversed edge
    //      0--<--3
    //     / \ B /   A: 2-0
    //    / A \ /    B: 0-2
    //   1-->--2
    auto reverse = std::find(newEdges.begin(), newEdges.end(), std::make_pair(faces[b], faces[a]));

    if (reverse != newEdges.end()) {
        newEdges.erase(reverse);
    } else {
        newEdges.emplace_back(faces[a], faces[b]);
    }

    return newEdges;
}

void RigidBody::solveCollision(const CollisionPoint &collision) {

    Vec3D velocity_perpendicular = collision.normal * velocity().dot(collision.normal);
    Vec3D velocity_parallel = velocity() - velocity_perpendicular;

    setVelocity(velocity_parallel);

    translate(-collision.normal * collision.depth);
}

void RigidBody::updatePhysicsState() {
    translate(_velocity * Time::deltaTime());
    _velocity = _velocity + _acceleration * Time::deltaTime();
}

void RigidBody::setVelocity(const Vec3D &velocity) {
    _velocity = velocity;
}

void RigidBody::addVelocity(const Vec3D &velocity) {
    _velocity = _velocity + velocity;
}

void RigidBody::setAcceleration(const Vec3D &acceleration) {
    _acceleration = acceleration;
}

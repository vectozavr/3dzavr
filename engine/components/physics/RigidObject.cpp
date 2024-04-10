#include <cmath>
#include <utility>

#include "RigidObject.h"
#include "utils/Time.h"
#include "utils/Log.h"
#include "Consts.h"

#include <iostream>


Vec3D RigidObject::findFurthestPoint(const Vec3D &direction) {
    Vec3D maxPoint{0, 0, 0};
    double maxDistance = -std::numeric_limits<double>::max();

    Matrix4x4 invModel = getComponent<TransformMatrix>()->fullInvModel();
    Vec3D modelDir = invModel*direction;

    for(auto & it : _hitBox) {
        double distance = it.dot(modelDir);

        if (distance > maxDistance) {
            maxDistance = distance;
            maxPoint = it;
        }
    }

    Matrix4x4 model = getComponent<TransformMatrix>()->fullModel();
    return Vec3D(model*maxPoint.makePoint4D());
}

Vec3D RigidObject::support(const std::shared_ptr<RigidObject>& obj, const Vec3D &direction) {
    Vec3D p1 = findFurthestPoint(direction);
    Vec3D p2 = obj->findFurthestPoint(-direction);

    return p1 - p2;
}

NextSimplex RigidObject::nextSimplex(const Simplex &points) {
    switch (points.type()) {
        case SimplexType::Line:
            return lineCase(points);
        case SimplexType::Triangle:
            return triangleCase(points);
        case SimplexType::Tetrahedron:
            return tetrahedronCase(points);

        default:
            throw std::logic_error{"RigidObject::nextSimplex: simplex is not Line, Triangle or Tetrahedron"};
    }
}

NextSimplex RigidObject::lineCase(const Simplex &points) {
    Simplex newPoints(points);
    Vec3D newDirection;

    const Vec3D& a = points[0];
    const Vec3D& b = points[1];

    Vec3D ab = b - a;
    Vec3D ao = -a;

    if (ab.dot(ao) > 0) {
        // TODO: sometimes the cross product returns zero (ab is parallel to ao)
        newDirection = ab.cross(ao).cross(ab);
        if(newDirection.abs() < Consts::EPS) {
            int asss = 0;
            newDirection = Vec3D(1);
        }

    } else {
        newPoints = Simplex{a};
        newDirection = ao;
    }

    return NextSimplex{newPoints, newDirection, false};
}

NextSimplex RigidObject::triangleCase(const Simplex &points) {
    Simplex newPoints(points);
    Vec3D newDirection;

    const Vec3D& a = points[0];
    const Vec3D& b = points[1];
    const Vec3D& c = points[2];

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ao = -a;

    Vec3D abc = ab.cross(ac);

    if(abc.abs() < Consts::EPS) {
        int asss = 0;
        abc = Vec3D(1);
    }

    if (abc.cross(ac).dot(ao) > 0) {
        if (ac.dot(ao) > 0) {
            newPoints = Simplex{a, c};
            newDirection = ac.cross(ao).cross(ac);

            if(newDirection.abs() < Consts::EPS) {
                int asss = 0;
            }
        } else {
            return lineCase(Simplex{a, b});
        }
    } else {
        if (ab.cross(abc).dot(ao) > 0) {
            return lineCase(Simplex{a, b});
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

NextSimplex RigidObject::tetrahedronCase(const Simplex &points) {
    const Vec3D& a = points[0];
    const Vec3D& b = points[1];
    const Vec3D& c = points[2];
    const Vec3D& d = points[3];

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ad = d - a;
    Vec3D ao = -a;

    Vec3D abc = ab.cross(ac);
    Vec3D acd = ac.cross(ad);
    Vec3D adb = ad.cross(ab);

    if(abc.abs() < Consts::EPS) {
        abc = Vec3D(1);
    }

    if(acd.abs() < Consts::EPS) {
        acd = Vec3D(1);
    }

    if(adb.abs() < Consts::EPS) {
        adb = Vec3D(1);
    }

    if (abc.dot(ao) > 0) {
        return triangleCase(Simplex{a, b, c});
    }

    if (acd.dot(ao) > 0) {
        return triangleCase(Simplex{a, c, d});
    }

    if (adb.dot(ao) > 0) {
        return triangleCase(Simplex{a, d, b});
    }

    return NextSimplex{points, Vec3D(), true};
}

std::pair<bool, Simplex> RigidObject::checkGJKCollision(const std::shared_ptr<RigidObject>& obj) {
    // This is implementation of GJK algorithm for collision detection.
    // It builds a simplex (simplest shape that can select point in space) around
    // zero for Minkowski Difference. Collision happened when zero point is inside.
    // See references:
    // https://www.youtube.com/watch?v=MDusDn8oTSE
    // https://blog.winter.dev/2020/gjk-algorithm/


    // Get initial support point in any direction
    Vec3D sup = support(obj, Vec3D{1, 0, 0});

    // Simplex is an array of points, max count is 4
    Simplex points{};
    points.push_front(sup);

    // New direction is towards the origin
    Vec3D direction = -sup;

    unsigned int iters = 0;
    while (iters++ < hitBoxSize() + obj->hitBoxSize()) {
        sup = support(obj, direction);

        if (sup.dot(direction) <= 0) {
            return std::make_pair(false, points); // no collision
        }

        points.push_front(sup);

        NextSimplex next = nextSimplex(points);

        direction = next.newDirection;
        points = next.newSimplex;

        if (next.finishSearching) {
            if (obj->hasCollision()) {
                obj->_inCollision = true;
            }
            _inCollision = true;
            return std::make_pair(true, points);
        }
    }
    return std::make_pair(false, points);
}

CollisionPoint RigidObject::EPA(const Simplex &simplex, std::shared_ptr<RigidObject> obj) {
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

    auto faceNormals = getFaceNormals(polytope, faces);
    std::vector<FaceNormal> normals = faceNormals.first;
    size_t minFace = faceNormals.second;

    Vec3D minNormal = normals[minFace].normal;
    double minDistance = std::numeric_limits<double>::max();

    size_t iters = 0;
    while (minDistance == std::numeric_limits<double>::max() && iters++ < hitBoxSize() + obj->hitBoxSize()) {
        minNormal = normals[minFace].normal;
        minDistance = normals[minFace].distance;

        Vec3D sup = support(obj, minNormal);
        double sDistance = minNormal.dot(sup);

        if (std::abs(sDistance - minDistance) > Consts::EPA_EPS) {
            minDistance = std::numeric_limits<double>::max();
            std::vector<std::pair<size_t, size_t>> uniqueEdges;

            long f = 0;
            for (size_t i = 0; i < normals.size(); i++) {
                /* TODO:
                 *  When checking what faces to delete you check if the normal of the triangle and the
                 *  support point are in the same direction. This only works for some cases.
                 *  Since you want to check if the support point is on one side of the triangle
                 *  you want to use the support point relative to the triangle. So the check would look like this
                 *  if (SameDirection(normals[i], sup-polytope[faces[i*3]])
                 *  But now it does not work really, so we need to understand why
                 */
                if (normals[i].normal.dot(sup) > 0) {
                    uniqueEdges = addIfUniqueEdge(uniqueEdges, faces, f + 0, f + 1);
                    uniqueEdges = addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                    uniqueEdges = addIfUniqueEdge(uniqueEdges, faces, f + 2, f + 0);

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
            polytope.push_back(sup);

            faces.insert(faces.end(), newFaces.begin(), newFaces.end());

            auto newFaceNormals = getFaceNormals(polytope, faces);

            normals = std::move(newFaceNormals.first);
            minFace = newFaceNormals.second;
        }
    }

    _collisionNormal = minNormal;
    if (std::abs(minDistance - std::numeric_limits<double>::max()) < Consts::EPS) {
        // TODO: sometimes we fall in here, but we have the collision.
        return CollisionPoint{minNormal, 0};
    }

    return CollisionPoint{minNormal, minDistance + Consts::EPA_EPS};
}

std::pair<std::vector<FaceNormal>, size_t>
RigidObject::getFaceNormals(const std::vector<Vec3D> &polytope, const std::vector<size_t> &faces) {
    std::vector<FaceNormal> normals;
    normals.reserve(faces.size() / 3);
    size_t nearestFaceIndex = 0;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < faces.size(); i += 3) {
        const Vec3D& a = polytope[faces[i + 0]];
        const Vec3D& b = polytope[faces[i + 1]];
        const Vec3D& c = polytope[faces[i + 2]];

        Vec3D normal = (b - a).cross(c - a).normalized();

        double distance = normal.dot(a);

        if (distance < 0) {
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
RigidObject::addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces,
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

void RigidObject::updatePhysicsState(double deltaTime) {
    _inCollision = false;
    getComponent<TransformMatrix>()->translate(_velocity * deltaTime);
    _velocity = _velocity + _acceleration * deltaTime;
}

void RigidObject::setVelocity(const Vec3D &velocity) {
    _velocity = velocity;
}

void RigidObject::addVelocity(const Vec3D &velocity) {
    _velocity = _velocity + velocity;
}

void RigidObject::setAcceleration(const Vec3D &acceleration) {
    _acceleration = acceleration;
}

bool RigidObject::initHitBox() {
    auto triangleMeshComponent = getComponent<TriangleMesh>();
    if(triangleMeshComponent) {
        _hitBox = HitBox(*triangleMeshComponent, _useSimpleBox);
        return true;
    }
    auto lineMeshComponent = getComponent<LineMesh>();
    if(lineMeshComponent) {
        _hitBox = HitBox(*lineMeshComponent, _useSimpleBox);
        return true;
    }

    return false;
}

void RigidObject::start() {
    if (!hasComponent<TransformMatrix>()) {
        // This component requires to work with TransformMatrix component,
        addComponent<TransformMatrix>();
    }
    initHitBox();
}

void RigidObject::fixedUpdate(double deltaTime) {
    if(_hitBox.empty()) {
        if(!initHitBox()) {
            return;
        }
    }

    updatePhysicsState(deltaTime);
}

void RigidObject::SolveCollision(const CollisionPoint &collision,
                                 const std::shared_ptr<RigidObject>& obj1,
                                 const std::shared_ptr<RigidObject>& obj2) {
    Vec3D velocity_perpendicular = collision.normal * obj1->velocity().dot(collision.normal);
    Vec3D velocity_parallel = obj1->velocity() - velocity_perpendicular;

    obj1->setVelocity(velocity_parallel);

    obj1->getComponent<TransformMatrix>()->translate(-collision.normal * collision.depth);

    // TODO: also add for the obj2
}

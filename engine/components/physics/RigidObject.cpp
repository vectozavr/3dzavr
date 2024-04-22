#include <cmath>
#include <utility>

#include <components/physics/RigidObject.h>
#include <utils/Log.h>
#include <Consts.h>


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

SupportPoint RigidObject::support(const std::shared_ptr<RigidObject>& obj, const Vec3D &direction) {
    Vec3D p1 = findFurthestPoint(direction);
    Vec3D p2 = obj->findFurthestPoint(-direction);

    return {p1, p2, p1 - p2};
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

    const Vec3D& a = points[0].support;
    const Vec3D& b = points[1].support;

    Vec3D ab = b - a;
    Vec3D ao = -a;

    if (ab.dot(ao) > 0) {
        newDirection = ab.cross(ao).cross(ab);

        // Sometimes the cross product returns zero, so we replace it with non-zero vector
        if(newDirection.abs() < Consts::EPS) {
            newDirection = Vec3D(1);
        }

    } else {
        newPoints = Simplex{points[0]};
        newDirection = ao;
    }

    return NextSimplex{newPoints, newDirection, false};
}

NextSimplex RigidObject::triangleCase(const Simplex &points) {
    Simplex newPoints(points);
    Vec3D newDirection;

    const Vec3D& a = points[0].support;
    const Vec3D& b = points[1].support;
    const Vec3D& c = points[2].support;

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ao = -a;

    Vec3D abc = ab.cross(ac);

    // Sometimes the cross product returns zero, so we replace it with non-zero vector
    if(abc.abs() < Consts::EPS) {
        abc = Vec3D(1);
    }

    if (abc.cross(ac).dot(ao) > 0) {
        if (ac.dot(ao) > 0) {
            newPoints = Simplex{points[0], points[2]};
            newDirection = ac.cross(ao).cross(ac);

            // Sometimes the cross product returns zero, so we replace it with non-zero vector
            if(newDirection.abs() < Consts::EPS) {
                newDirection = Vec3D(1);
            }
        } else {
            return lineCase(Simplex{points[0], points[1]});
        }
    } else {
        if (ab.cross(abc).dot(ao) > 0) {
            return lineCase(Simplex{points[0], points[1]});
        } else {
            if (abc.dot(ao) > 0) {
                newDirection = abc;
            } else {
                newPoints = Simplex{points[0], points[2], points[1]};
                newDirection = -abc;
            }
        }
    }

    return NextSimplex{newPoints, newDirection, false};
}

NextSimplex RigidObject::tetrahedronCase(const Simplex &points) {
    const Vec3D& a = points[0].support;
    const Vec3D& b = points[1].support;
    const Vec3D& c = points[2].support;
    const Vec3D& d = points[3].support;

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ad = d - a;
    Vec3D ao = -a;

    Vec3D abc = ab.cross(ac);
    Vec3D acd = ac.cross(ad);
    Vec3D adb = ad.cross(ab);

    // Sometimes the cross product returns zero, so we replace it with non-zero vector
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
        return triangleCase(Simplex{points[0], points[1], points[2]});
    }

    if (acd.dot(ao) > 0) {
        return triangleCase(Simplex{points[0], points[2], points[3]});
    }

    if (adb.dot(ao) > 0) {
        return triangleCase(Simplex{points[0], points[3], points[1]});
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
    SupportPoint sup = support(obj, Vec3D{1, 0, 0});

    // Simplex is an array of points, max count is 4
    Simplex points{};
    points.push_front(sup);

    // New direction is towards the origin
    Vec3D direction = -sup.support;

    unsigned int iters = 0;
    while (iters++ < std::min(hitBoxSize() + obj->hitBoxSize(), (size_t)Consts::GJK_MAX_ITERATIONS)) {
        sup = support(obj, direction);

        if (sup.support.dot(direction) <= 0) {
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

    std::vector<SupportPoint> polytope(simplex.begin(), simplex.end());
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
    while (minDistance == std::numeric_limits<double>::max() && iters++ < std::min(hitBoxSize() + obj->hitBoxSize(), (size_t)Consts::EPA_MAX_ITERATIONS)) {
        minNormal = normals[minFace].normal;
        minDistance = normals[minFace].distance;

        SupportPoint sup = support(obj, minNormal);
        double sDistance = minNormal.dot(sup.support);

        if (std::abs(sDistance - minDistance) > Consts::EPS) {
            minDistance = std::numeric_limits<double>::max();
            std::vector<std::pair<size_t, size_t>> uniqueEdges;

            long f = 0;
            for (size_t i = 0; i < normals.size(); i++) {
                if (normals[i].normal.dot(sup.support-polytope[faces[f]].support) > 0) {
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

    double penetrationDepth = minDistance + Consts::EPA_DEPTH_EPS;
    if (std::abs(minDistance - std::numeric_limits<double>::max()) < Consts::EPS) {
        penetrationDepth = 0;
    }

    Vec3D collisionPoint = calculateCollisionPoint(polytope[faces[minFace*3]],
                                                   polytope[faces[minFace*3+1]],
                                                   polytope[faces[minFace*3+2]],
                                                   minNormal);

    return CollisionPoint{
            collisionPoint,
        minNormal,
        penetrationDepth,
        polytope[faces[minFace*3]],
        polytope[faces[minFace*3+1]],
        polytope[faces[minFace*3+2]],
        polytope};
}

std::pair<std::vector<FaceNormal>, size_t>
RigidObject::getFaceNormals(const std::vector<SupportPoint> &polytope, const std::vector<size_t> &faces) {
    std::vector<FaceNormal> normals;
    normals.reserve(faces.size() / 3);
    size_t nearestFaceIndex = 0;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < faces.size(); i += 3) {
        const Vec3D& a = polytope[faces[i + 0]].support;
        const Vec3D& b = polytope[faces[i + 1]].support;
        const Vec3D& c = polytope[faces[i + 2]].support;

        Vec3D normal = (b - a).cross(c - a).normalized();

        double distance = normal.dot(a);

        if (distance < 0) {
            normal *= -1;
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
        computeCenterOfMass(triangleMeshComponent);
        computeInertiaTensor(triangleMeshComponent);
        return true;
    }
    auto lineMeshComponent = getComponent<LineMesh>();
    if(lineMeshComponent) {
        _hitBox = HitBox(*lineMeshComponent, _useSimpleBox);
        computeCenterOfMass(lineMeshComponent);
        computeInertiaTensor(lineMeshComponent);
        return true;
    }

    return false;
}

void RigidObject::computeCenterOfMass(const std::shared_ptr<TriangleMesh>& triangleMesh) {
    Vec3D weightedCentroidSum(0);
    _volume = 0.0;
    _surfaceArea = 0.0;

    for (const auto& triangle : triangleMesh->triangles()) {
        Vec3D v1(triangle[0]);
        Vec3D v2(triangle[1]);
        Vec3D v3(triangle[2]);

        Vec3D centroid = (v1 + v2 + v3)/3;

        // Assuming the reference point O is the origin (0, 0, 0) for simplicity
        double dVolume = v1.dot(v2.cross(v3))/6;

        weightedCentroidSum += centroid * dVolume;
        _volume += dVolume;

        _surfaceArea += ((v2-v1).cross(v3-v1)).abs()/2;
    }
    _centerOfMass = weightedCentroidSum / _volume;
}

void RigidObject::computeInertiaTensor(const std::shared_ptr<TriangleMesh>& triangleMesh) {
    _inertiaTensor = Matrix3x3::Zero();

    for (const auto& triangle : triangleMesh->triangles()) {
        Vec3D v1(triangle[0]);
        Vec3D v2(triangle[1]);
        Vec3D v3(triangle[2]);

        Vec3D centroid = (v1 + v2 + v3)/3;

        double dVolume = v1.dot(v2.cross(v3))/6;

        // Position vector from the origin to the centroid, then to the actual COM of the tetrahedron
        Vec3D r = centroid/4 - _centerOfMass;

        // For uniform density, distribute the total mass according to the volume ratio
        double dMass = _mass * (dVolume / _volume);

        // Here we assume that entire dMass is located in centroid/4
        _inertiaTensor += (Matrix3x3::Identity()*r.sqrAbs() - Matrix3x3::Outer(r, r))*dMass;
    }
    _invInertiaTensor = _inertiaTensor.inverse();
}

void RigidObject::computeCenterOfMass(const std::shared_ptr<LineMesh>& lineMesh) {
    // TODO
}

void RigidObject::computeInertiaTensor(const std::shared_ptr<LineMesh>& lineMesh) {
    // TODO
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
    // This is the old version without physics simulations
    /*
    Vec3D velocity_perpendicular = collision.normal * obj1->velocity().dot(collision.normal);
    Vec3D velocity_parallel = obj1->velocity() - velocity_perpendicular;
    obj1->setVelocity(velocity_parallel);
    */

    obj1->getComponent<TransformMatrix>()->translate(-collision.normal * collision.depth);

    // TODO: implement Rigid Body physics
    /*
    Vec3D rA = collision. - objA.position;
    Vec3D rB = collision. - objB.position;

    glm::vec3 relativeVelocity = objB.velocity + glm::cross(objB.angularVelocity, rB) -
                                 objA.velocity - glm::cross(objA.angularVelocity, rA);

    float velocityAlongNormal = glm::dot(relativeVelocity, collision.contactNormal);

    if (velocityAlongNormal > 0) continue; // Objects moving apart

    float e = std::min(objA.restitution, objB.restitution);

    float numerator = -(1 + e) * velocityAlongNormal;
    float denominator = objA.mass + objB.mass +
                        glm::dot(collision.contactNormal,
                                 glm::cross(objA.inertiaTensorInverse * glm::cross(rA, collision.contactNormal), rA) +
                                 glm::cross(objB.inertiaTensorInverse * glm::cross(rB, collision.contactNormal), rB));

    float j = numerator / denominator;

    glm::vec3 impulse = j * collision.contactNormal;

    objA.velocity -= impulse / objA.mass;
    objB.velocity += impulse / objB.mass;

    objA.angularVelocity -= objA.inertiaTensorInverse * glm::cross(rA, impulse);
    objB.angularVelocity += objB.inertiaTensorInverse * glm::cross(rB, impulse);
     */
}

Vec3D
RigidObject::calculateCollisionPoint(const SupportPoint &edge1,
                                     const SupportPoint &edge2,
                                     const SupportPoint &edge3,
                                     const Vec3D& normal) {
    /*TODO: this function calculate only one single point of contact.
     * Unfortunately, this point is not unique: there might be several
     * points of contact. Ideally we should find all of them, but of course this will
     * reduce performance.
     * More info you can read here (Russian article on how to find all points of contact):
     * https://habr.com/ru/articles/753776/
     */

    Triangle nearestFace({
        edge1.support.makePoint4D(),
        edge2.support.makePoint4D(),
        edge3.support.makePoint4D()
    });

    double d = edge1.support.dot(normal);
    Vec3D proj = normal*d;
    auto abg = nearestFace.abgBarycCoord(proj);

    auto p1 = edge1.p1*abg.x() + edge2.p1*abg.y() + edge3.p1*abg.z();
    auto p2 = edge1.p2*abg.x() + edge2.p2*abg.y() + edge3.p2*abg.z();

    return (p1 + p2)/2;
}

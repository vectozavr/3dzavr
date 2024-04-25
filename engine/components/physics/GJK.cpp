#include <components/physics/GJK.h>
#include <components/physics/RigidObject.h>

SupportPoint GJK::support(const std::shared_ptr<RigidObject>& obj1,
                          const std::shared_ptr<RigidObject>& obj2,
                          const Vec3D &direction) {
    Vec3D p1 = obj1->findFurthestPoint(direction);
    Vec3D p2 = obj2->findFurthestPoint(-direction);

    return {p1, p2, p1 - p2};
}

GJK::NextSimplex GJK::nextSimplex(const Simplex &points) {
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

GJK::NextSimplex GJK::lineCase(const Simplex &points) {
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

GJK::NextSimplex GJK::triangleCase(const Simplex &points) {
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

GJK::NextSimplex GJK::tetrahedronCase(const Simplex &points) {
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

std::pair<bool, Simplex> GJK::checkGJKCollision(const std::shared_ptr<RigidObject>& obj1,
                                                const std::shared_ptr<RigidObject>& obj2) {
    // This is implementation of GJK algorithm for collision detection.
    // It builds a simplex (simplest shape that can select point in space) around
    // zero for Minkowski Difference. Collision happened when zero point is inside.
    // See references:
    // https://www.youtube.com/watch?v=MDusDn8oTSE
    // https://blog.winter.dev/2020/gjk-algorithm/


    // Get initial support point in any direction
    SupportPoint sup = support(obj1, obj2, Vec3D{1, 0, 0});

    // Simplex is an array of points, max count is 4
    Simplex points{};
    points.push_front(sup);

    // New direction is towards the origin
    Vec3D direction = -sup.support;

    unsigned int iters = 0;
    while (iters++ < std::min(obj1->hitBoxSize() + obj2->hitBoxSize(), (size_t)Consts::GJK_MAX_ITERATIONS)) {
        sup = support(obj1, obj2, direction);

        if (sup.support.dot(direction) <= 0) {
            return std::make_pair(false, points); // no collision
        }

        points.push_front(sup);

        NextSimplex next = nextSimplex(points);

        direction = next.newDirection;
        points = next.newSimplex;

        if (next.finishSearching) {
            if (obj1->hasCollision()) {
                obj1->setInCollision();
            }
            if(obj2->hasCollision()) {
                obj2->setInCollision();
            }
            return std::make_pair(true, points);
        }
    }
    return std::make_pair(false, points);
}
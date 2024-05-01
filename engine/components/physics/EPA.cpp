#include <components/physics/EPA.h>
#include <components/physics/RigidObject.h>

EPA::CollisionPoint EPA::EPA(std::shared_ptr<RigidObject> obj1,
                             std::shared_ptr<RigidObject> obj2,
                             const Simplex &simplex) {
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
    std::vector<GJK::FaceNormal> normals = faceNormals.first;
    size_t minFace = faceNormals.second;

    Vec3D minNormal = normals[minFace].normal;
    double minDistance = std::numeric_limits<double>::max();

    size_t iters = 0;
    while (minDistance == std::numeric_limits<double>::max() && iters++ < std::min(obj1->hitBoxSize() + obj2->hitBoxSize(), (size_t)Consts::EPA_MAX_ITERATIONS)) {
        minNormal = normals[minFace].normal;
        minDistance = normals[minFace].distance;

        SupportPoint sup = GJK::support(obj1, obj2, minNormal);
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

    double penetrationDepth = minDistance + Consts::EPA_DEPTH_EPS;
    if (std::abs(minDistance - std::numeric_limits<double>::max()) < Consts::EPS) {
        penetrationDepth = 0;
    }

    //Vec3D collisionPoint = calculateCollisionPoint(polytope[faces[minFace*3]],
    //                                               polytope[faces[minFace*3+1]],
    //                                               polytope[faces[minFace*3+2]],
    //                                               minNormal);

    solveCollision(obj1, obj2, minNormal, penetrationDepth);

    auto collisionPoints = calculateCollisionPoints(obj1, obj2, minNormal);

    return CollisionPoint{
            collisionPoints,
            minNormal,
            penetrationDepth,
            polytope[faces[minFace*3]],
            polytope[faces[minFace*3+1]],
            polytope[faces[minFace*3+2]],
            polytope};
}

std::pair<std::vector<GJK::FaceNormal>, size_t> EPA::getFaceNormals(
        const std::vector<SupportPoint> &polytope, const std::vector<size_t> &faces) {

    std::vector<GJK::FaceNormal> normals;
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

        normals.emplace_back(GJK::FaceNormal{normal, distance});

        if (distance < minDistance) {
            nearestFaceIndex = i / 3;
            minDistance = distance;
        }
    }

    return {normals, nearestFaceIndex};
}

std::vector<std::pair<size_t, size_t>> EPA::addIfUniqueEdge(
        const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces, size_t a, size_t b) {

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

Vec3D EPA::calculateSingleCollisionPoint(const SupportPoint &edge1,
                                         const SupportPoint &edge2,
                                         const SupportPoint &edge3,
                                         const Vec3D& normal) {
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


void EPA::solveCollision(std::shared_ptr<RigidObject> obj1, std::shared_ptr<RigidObject> obj2, const Vec3D& normal, double depth) {
    if(obj1->hasCollision() && !obj2->hasCollision()) { // translate only the first object
        obj1->getComponent<TransformMatrix>()->translate(-normal*depth);
    }
    if(!obj1->hasCollision() && obj2->hasCollision()) { // translate only the second object
        obj2->getComponent<TransformMatrix>()->translate(normal*depth);
    }
    if(obj1->hasCollision() && obj2->hasCollision()) { // we should translate both objects
        double dp1 = (obj1->getComponent<TransformMatrix>()->fullPosition() - obj1->lastPosition()).abs();
        double dp2 = (obj2->getComponent<TransformMatrix>()->fullPosition() - obj2->lastPosition()).abs();

        // We translate objects based on how much their coordinates changed since the last frame
        double sum = dp1 + dp2;
        double alpha = 0.5;
        double betta = 0.5;
        if(sum > Consts::EPS) {
            alpha = dp1 / (sum);
            betta = 1 - alpha;
        }

        obj1->getComponent<TransformMatrix>()->translate(-normal*depth*alpha);
        obj2->getComponent<TransformMatrix>()->translate(normal*depth*betta);
    }
}

std::vector<Vec3D> EPA::PlanePoints::intersectionsWithLine(const Vec3D &a, const Vec3D &b) const {
    std::vector<Vec3D> result;

    for(int i = 0; i < points.size(); i++) {
        Vec3D o = points[i];
        Vec3D c = points[(i+1)%points.size()];
        Vec3D oc = c - o;
        Vec3D oa = a - o;
        Vec3D ob = b - o;


        if(oa.dot(normals[i])*ob.dot(normals[i]) < 0) {
            // there might be a collision
            auto intersection = Vec3D::intersectionOfLines(a, b, o, c);
            Vec3D p = intersection.first;
            Vec3D op = p - o;
            if(op.dot(oc) > 0 && op.sqrAbs() < oc.sqrAbs()) {
                result.emplace_back(p);
            }
        }
    }

    return result;
}

void EPA::PlanePoints::computeNormals() {
    normals.reserve(points.size());
    for(int i = 0; i < points.size(); i++) {
        Vec3D oa = points[i] - origin;
        Vec3D ab = points[(i+1)%points.size()] - points[i];
        Vec3D abNormalized = ab.normalized();

        Vec3D lineNorm = oa - abNormalized*oa.dot(abNormalized);
        lineNorm = lineNorm.normalized();

        normals.emplace_back(lineNorm);
    }
}

EPA::PlanePoints EPA::findSortedPointsFromPlane(std::shared_ptr<RigidObject> obj, const Plane& plane) {
    EPA::PlanePoints result;

    std::vector<std::pair<Vec3D, double>> planePoints;
    Vec3D origin(0, 0, 0);

    auto model = obj->getComponent<TransformMatrix>()->fullModel();

    for(const auto& p: obj->hitBox()) {
        Vec3D pInWorld = Vec3D(model*p.makePoint4D());
        if (std::abs(plane.distance(pInWorld)) < Consts::EPA_CONTACT_PLANE_DISTANCE_EPS) {
            // Compute the projection of pInWorld onto the collision plane
            Vec3D ap = pInWorld - plane.point;
            Vec3D ap_parallel = ap - plane.normal*ap.dot(plane.normal);
            Vec3D pInWorldProjected = plane.point + ap_parallel;

            planePoints.emplace_back(pInWorldProjected, 0);
            origin += pInWorldProjected;
        }
    }
    if(planePoints.empty()) {
        return result;
    }

    origin /= planePoints.size();
    result.origin = origin;

    // sort points clockwise
    planePoints[0].second = 1.0;
    for(int i = 1; i < planePoints.size(); i++) {
        Vec3D oa = planePoints[0].first - origin;
        Vec3D ob = planePoints[i].first - origin;
        double cosAngle = oa.dot(ob)/(oa.abs()*ob.abs());
        double sign = oa.cross(ob).dot(plane.normal);

        if(sign > 0) {
            planePoints[i].second = cosAngle;
        } else {
            planePoints[i].second = -cosAngle - 2;
        }
    }

    std::sort(planePoints.begin(), planePoints.end(), [](const auto& p1, const auto& p2) {
        return p1.second > p2.second;
    });

    // Place sorted points into the final structure
    result.points.reserve(planePoints.size());
    for(auto & planePoint : planePoints) {
        result.points.emplace_back(planePoint.first);
    }

    // Compute normals for the obtained 2D convex polygon
    result.computeNormals();

    return result;
}

bool EPA::PlanePoints::isPointInside(const Vec3D &point) const {
    if(points.size() < 3) {
        return false;
    }

    for(int i = 0; i < points.size(); i++) {
        Vec3D op = point - points[i];
        if(op.dot(normals[i]) > 0) {
            return false;
        }
    }

    return true;
}

void EPA::PlanePoints::computeOrigin() {
    origin = Vec3D(0,0,0);
    for (const auto&p : points) {
        origin += p;
    }
    origin /= points.size();
}

EPA::PlanePoints EPA::findPlanePointsIntersection(const EPA::PlanePoints& points1, const EPA::PlanePoints& points2) {
    std::vector<Vec3D> resultPoints;

    bool aInside = points2.isPointInside(points1.points[0]);
    for(int i = 0; i < points1.points.size(); i++) {
        if((points1.points.size() < 3) && i > 0){
            break; // we do not need to count the same line two times...
        }

        Vec3D a = points1.points[i];
        Vec3D b = points1.points[(i+1)%points1.points.size()];

        bool bInside = points2.isPointInside(b);

        if(aInside) {
            resultPoints.emplace_back(a);
        }

        if(!aInside || !bInside) {
            auto newPoints = points2.intersectionsWithLine(a, b);
            resultPoints.insert(resultPoints.end(), newPoints.begin(), newPoints.end());
        }

        aInside = bInside; // the next a will be the current b
    }

    std::set<Vec3D> seenPoints(resultPoints.begin(), resultPoints.end());
    for(const auto& p : points2.points) {
        if(points1.isPointInside(p) && !seenPoints.contains(p)) {
            resultPoints.emplace_back(p);
        }
    }

    EPA::PlanePoints result;
    result.points = resultPoints;
    result.computeOrigin();
    result.computeNormals();

    return result;
}


EPA::PlanePoints EPA::calculateCollisionPoints(std::shared_ptr<RigidObject> obj1,
                                                 std::shared_ptr<RigidObject> obj2,
                                                 const Vec3D& normal) {
    std::vector<Vec3D> result;

    Vec3D onePointFromCollisionPlane = obj1->findFurthestPoint(normal);
    Plane collisionPlane(normal, onePointFromCollisionPlane);

    auto obj1PlanePoints = findSortedPointsFromPlane(obj1, collisionPlane);
    auto obj2PlanePoints = findSortedPointsFromPlane(obj2, collisionPlane);

    if(obj1PlanePoints.points.size() == 1) {
        return {{obj1PlanePoints.points[0]}, {}, obj1PlanePoints.points[0]}; // edge to face/vertex
    }
    if(obj2PlanePoints.points.size() == 1) {
        return {{obj2PlanePoints.points[0]}, {}, obj2PlanePoints.points[0]}; // edge to face/vertex
    }
    if(obj1PlanePoints.points.size() == 2 && obj2PlanePoints.points.size() == 2) { // vertex to vertex
        Vec3D intersection = Vec3D::intersectionOfLines(obj1PlanePoints.points[0], obj1PlanePoints.points[1],
                                                        obj2PlanePoints.points[0], obj2PlanePoints.points[1]).first;
        return {{intersection}, {}, intersection};
    }

    return findPlanePointsIntersection(obj1PlanePoints, obj2PlanePoints);
}

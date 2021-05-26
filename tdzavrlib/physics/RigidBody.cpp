//
// Created by Иван Ильин on 05.02.2021.
//

#include "RigidBody.h"
#include "../Plane.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include <iostream>
#include <cmath>

Point4D RigidBody::_findFurthestPoint(const Point4D& direction) {
    std::vector<Point4D> visitedPoints = {};

    Point4D maxPoint = {};
    auto maxDistance = (double)-INFINITY;
    for(auto& tri : triangles()){
        for(auto point : tri.p){

            point += position();

            // additional check if we already was in this point (in mesh we often repeat the same points)
            /*
            for(auto& visited : visitedPoints)
                if((visited - point).abs() < 0.001)
                    continue;
            visitedPoints.push_back(point);
            */

            double distance = point.dot(direction);
            if(distance > maxDistance) {
                maxDistance = distance;
                maxPoint = point;
            }
        }
    }
    return maxPoint;
}

Point4D RigidBody::_support(const std::shared_ptr<RigidBody>& obj, const Point4D& direction) {
    Point4D p1 = _findFurthestPoint(direction);
    Point4D p2 = obj->_findFurthestPoint(-direction);

    return p1 - p2;
}

bool RigidBody::_nextSimplex(Simplex &points, Point4D &direction) {
    switch (points.size()) {
        case 2: return _line(points, direction);
        case 3: return _triangle(points, direction);
        case 4: return _tetrahedron(points, direction);
    }

    // never should be here
    return false;
}

bool RigidBody::_line(Simplex& points, Point4D& direction) {
    Point4D a = points[0];
    Point4D b = points[1];

    Point4D ab = b - a;
    Point4D ao =   - a;

    if (ab.dot(ao) > 0) {
        direction = ab.cross3D(ao).cross3D(ab);
    } else {
        points = { a };
        direction = ao;
    }

    return false;
}

bool RigidBody::_triangle(Simplex &points, Point4D &direction) {
    Point4D a = points[0];
    Point4D b = points[1];
    Point4D c = points[2];

    Point4D ab = b - a;
    Point4D ac = c - a;
    Point4D ao =   - a;

    Point4D abc = ab.cross3D(ac);

    if (abc.cross3D(ac).dot(ao) > 0) {
        if (ac.dot(ao) > 0) {
            points = { a, c };
            direction = ac.cross3D(ao).cross3D(ac);
        }
        else
        {
            return _line(points = { a, b }, direction);
        }
    }
    else
    {
        if (ab.cross3D(abc).dot(ao) > 0) {
            return _line(points = { a, b }, direction);
        }
        else
        {
            if (abc.dot(ao) > 0) {
                direction = abc;
            } else {
                points = { a, c, b };
                direction = -abc;
            }
        }
    }

    return false;
}

bool RigidBody::_tetrahedron(Simplex &points, Point4D &direction) {
    Point4D a = points[0];
    Point4D b = points[1];
    Point4D c = points[2];
    Point4D d = points[3];

    Point4D ab = b - a;
    Point4D ac = c - a;
    Point4D ad = d - a;
    Point4D ao =   - a;

    Point4D abc = ab.cross3D(ac);
    Point4D acd = ac.cross3D(ad);
    Point4D adb = ad.cross3D(ab);

    if (abc.dot(ao) > 0) {
        return _triangle(points = { a, b, c }, direction);
    }

    if (acd.dot(ao) > 0) {
        return _triangle(points = { a, c, d }, direction);
    }

    if (adb.dot(ao) > 0) {
        return _triangle(points = { a, d, b }, direction);
    }

    return true;
}

std::pair<bool, Simplex> RigidBody::checkGJKCollision(const std::shared_ptr<RigidBody>& obj) {

    // Get initial support point in any direction
    Point4D support = _support(obj, Point4D::unit_x());

    // Simplex is an array of points, max count is 4
    Simplex points;
    points.push_front(support);

    // New direction is towards the origin
    Point4D direction = -support;

    while (true) {
        support = _support(obj, direction);

        if (support.dot(direction) <= 0)
            return std::make_pair(false, points); // no collision

        points.push_front(support);

        if (_nextSimplex(points, direction)) {
            _inCollision = true;
            return std::make_pair(true, points);
        }
    }
}

CollisionPoint RigidBody::EPA(const Simplex& simplex, const std::shared_ptr<RigidBody>& obj) {

    std::vector<Point4D> polytope(simplex.begin(), simplex.end());
    std::vector<size_t>  faces = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2
    };

    // list: vector4(normal, distance), index: min distance
    auto [normals, minFace] = GetFaceNormals(polytope, faces);

    Point4D minNormal;
    double minDistance = INFINITY;

    int iterations = 0;

    while ((minDistance == INFINITY) && (iterations < 50)) {
        minNormal   = normals[minFace];
        minDistance = normals[minFace].w;

        Point4D support = _support(obj, minNormal);
        double sDistance = minNormal.dot(support);

        if (abs(sDistance - minDistance) > 0.0001) {
            minDistance = INFINITY;
            std::vector<std::pair<size_t, size_t>> uniqueEdges;

            for (size_t i = 0; i < normals.size(); i++) {
                if (normals[i].dot(support) > 0) {
                    size_t f = i * 3;

                    AddIfUniqueEdge(uniqueEdges, faces, f,     f + 1);
                    AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                    AddIfUniqueEdge(uniqueEdges, faces, f + 2, f    );

                    faces[f + 2] = faces.back(); faces.pop_back();
                    faces[f + 1] = faces.back(); faces.pop_back();
                    faces[f    ] = faces.back(); faces.pop_back();

                    normals[i] = normals.back(); normals.pop_back();

                    i--;
                }
            }
            std::vector<size_t> newFaces;
            for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
                newFaces.push_back(edgeIndex1);
                newFaces.push_back(edgeIndex2);
                newFaces.push_back(polytope.size());
            }

            polytope.push_back(support);

            auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);

            if(newNormals.empty())
                break;

            double oldMinDistance = INFINITY;
            for (size_t i = 0; i < normals.size(); i++) {
                if (normals[i].w < oldMinDistance) {
                    oldMinDistance = normals[i].w;
                    minFace = i;
                }
            }

            if (newNormals[newMinFace].w < oldMinDistance) {
                minFace = newMinFace + normals.size();
            }

            faces  .insert(faces  .end(), newFaces  .begin(), newFaces  .end());
            normals.insert(normals.end(), newNormals.begin(), newNormals.end());
        }
        iterations++;
    }
    CollisionPoint points;

    points.normal = minNormal;
    points.depth = minDistance + 0.0001;
    points.hasCollision = minDistance < INFINITY;

    return points;
}

std::pair<std::vector<Point4D>, size_t> RigidBody::GetFaceNormals(const std::vector<Point4D>& polytope, const std::vector<size_t>&  faces) {
    std::vector<Point4D> normals;
    size_t minTriangle = 0;
    double minDistance = INFINITY;

    for (size_t i = 0; i < faces.size(); i += 3) {
        Point4D a = polytope[faces[i    ]];
        Point4D b = polytope[faces[i + 1]];
        Point4D c = polytope[faces[i + 2]];

        Point4D normal = (b - a).cross3D(c - a).normalize();
        double distance = normal.dot(a);

        if (distance < 0) {
            normal   *= -1;
            distance *= -1;
        }

        normal.w = distance;
        normals.emplace_back(normal);

        if (distance < minDistance) {
            minTriangle = i / 3;
            minDistance = distance;
        }
    }

    return { normals, minTriangle };
}

void RigidBody::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b) {

    auto reverse = std::find(                   //      0--<--3
            edges.begin(),                      //     / \ B /   A: 2-0
            edges.end(),                        //    / A \ /    B: 0-2
            std::make_pair(faces[b], faces[a])  //   1-->--2
    );

    if (reverse != edges.end()) {
        edges.erase(reverse);
    }

    else {
        edges.emplace_back(faces[a], faces[b]);
    }
}

void RigidBody::updatePhysicsState() {
    translate(p_velocity * Time::deltaTime());
    p_velocity += p_acceleration * Time::deltaTime();

    rotate(p_angularVelocity * Time::deltaTime());
    p_angularVelocity += p_angularAcceleration * Time::deltaTime();
}

void RigidBody::setVelocity(const Point4D& velocity) {
    p_velocity = velocity;
}

void RigidBody::setAngularVelocity(const Point4D& angularVelocity) {
    p_angularVelocity = angularVelocity;
}

void RigidBody::setAcceleration(const Point4D& acceleration) {
    p_acceleration = acceleration;
}

void RigidBody::setAngularAcceleration(const Point4D& angularAcceleration) {
    p_angularAcceleration = angularAcceleration;
}

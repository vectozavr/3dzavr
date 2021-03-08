//
// Created by Иван Ильин on 05.02.2021.
//

#include "RigidBody.h"
#include "../Plane.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include <iostream>

bool RigidBody::checkGJKCollision(RigidBody &obj) {

    // Get initial support point in any direction
    Point4D support = _support(obj, Point4D::unit_x());

    // Simplex is an array of points, max count is 4
    Simplex points;
    points.push_front(support);

    // New direction is towards the origin
    Point4D direction = -support;

    while (true) {
        support = _support(obj, direction);

        if (support.dot(direction) <= 0) {
            return false; // no collision
        }

        points.push_front(support);
        if (_nextSimplex(points, direction)) {
            return true;
        }
    }
}

void RigidBody::updatePhysicsState() {
    translate(p_velocity * Time::deltaTime());
    p_velocity += p_acceleration * Time::deltaTime();

    rotate(p_angularVelocity * Time::deltaTime());
    p_angularVelocity += p_angularAcceleration * Time::deltaTime();
}

void RigidBody::applyVelocity(const Point4D& velocity) {
    p_velocity = velocity;
}

void RigidBody::applyAngularVelocity(const Point4D& angularVelocity) {
    p_angularVelocity = angularVelocity;
}

void RigidBody::applyAcceleration(const Point4D& acceleration) {
    p_acceleration = acceleration;
}

void RigidBody::applyAngularAcceleration(const Point4D& angularAcceleration) {
    p_angularAcceleration = angularAcceleration;
}

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

Point4D RigidBody::_support(RigidBody &obj, const Point4D& direction) {
    Point4D p1 = _findFurthestPoint(direction);
    Point4D p2 = obj._findFurthestPoint(-direction);

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


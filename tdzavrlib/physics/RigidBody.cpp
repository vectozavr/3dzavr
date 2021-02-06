//
// Created by Иван Ильин on 05.02.2021.
//

#include "RigidBody.h"
#include "../Plane.h"
#include "../utils/Log.h"
#include "../utils/Time.h"

bool RigidBody::checkPointInsideTriangle(const Point4D& point, const Point4D& position, const Triangle& triangle) {
    Point4D norm = triangle.norm();

    Point4D v0 = triangle[1] - triangle[0];
    Point4D v1 = triangle[2] - triangle[1];
    Point4D v2 = triangle[0] - triangle[2];

    Point4D s0 = point - (position + triangle[0]);
    Point4D s1 = point - (position + triangle[1]);
    Point4D s2 = point - (position + triangle[2]);
    if((s0.cross3D(v0).dot(norm) <= 0) && (s1.cross3D(v1).dot(norm) <= 0) && (s2.cross3D(v2).dot(norm) <= 0))
        return true;
    return false;
}

bool RigidBody::checkCollision(const RigidBody &obj) {
    bool wasCollision = false;
    for(auto& triangle : triangles()){
        for(auto& objTriangle : obj.triangles()) {
            Point4D norm = objTriangle.norm();
            Plane plane(norm, obj.position() + objTriangle[0]);

            Point4D insidePoints[3];  int inside = 0;
            Point4D outsidePoints[3]; int outside = 0;

            double distances[3] = {plane.distance(position() + triangle[0]), plane.distance(position() + triangle[1]), plane.distance(position() + triangle[2])};

            for(int i = 0; i < 3; i++) {
                if (distances[i] >= 0)
                    insidePoints[inside++] = triangle[i] + position();
                else
                    outsidePoints[outside++] = triangle[i] + position();
            }

            std::pair<Point4D, double> intersect1;
            std::pair<Point4D, double> intersect2;

            if(inside == 1) {
                intersect1 = plane.overIntersection(insidePoints[0], outsidePoints[0]);
                intersect2 = plane.overIntersection(insidePoints[0], outsidePoints[1]);
                //Point4D insidePoint = insidePoints[0];
            }

            if(inside == 2) {
                intersect1 = plane.overIntersection(insidePoints[0], outsidePoints[0]);
                intersect2 = plane.overIntersection(insidePoints[1], outsidePoints[0]);
            }

            if(checkPointInsideTriangle(intersect1.first, obj.position(), objTriangle) || checkPointInsideTriangle(intersect2.first, obj.position(), objTriangle)) {
                p_velocity *= 0;
                applyAcceleration({0, 0, 0});
                wasCollision = true;
            }

        }
    }
    return wasCollision;
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


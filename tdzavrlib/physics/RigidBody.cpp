//
// Created by Иван Ильин on 05.02.2021.
//

#include "RigidBody.h"
#include "../Plane.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include <iostream>

bool RigidBody::checkPointInsideTriangle(const Point4D& point, const Point4D& position, const Triangle& triangle) {
    Point4D norm = triangle.norm();

    Point4D v0 = triangle[1] - triangle[0];
    Point4D v1 = triangle[2] - triangle[1];
    Point4D v2 = triangle[0] - triangle[2];

    Point4D s0 = point - (position + triangle[0]);
    Point4D s1 = point - (position + triangle[1]);
    Point4D s2 = point - (position + triangle[2]);
    if((s0.cross3D(v0).dot(norm) < 0) && (s1.cross3D(v1).dot(norm) < 0) && (s2.cross3D(v2).dot(norm) < 0))
        return true;
    return false;
}

bool RigidBody::checkCollision(RigidBody &obj) {
    Point4D translation = {0,0,0};
    int collisions = 0;
    bool trianglesAreParallel = false;
    for(auto& triangle : triangles()){
        for(auto& objTriangle : obj.triangles()) {
            Point4D norm = objTriangle.norm();
            double trianglesNormDot =  triangle.norm().dot(norm);

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
            std::pair<Point4D, double> intersect3;


            if(abs(abs(trianglesNormDot) - 1) > 0.01) {
                trianglesAreParallel = false;

                if (inside == 0 || inside == 3)
                    continue;

                if (inside == 1) {
                    intersect1 = plane.overIntersection(insidePoints[0], outsidePoints[0]);
                    intersect2 = plane.overIntersection(insidePoints[0], outsidePoints[1]);
                }

                if (inside == 2) {
                    intersect1 = plane.overIntersection(insidePoints[0], outsidePoints[0]);
                    intersect2 = plane.overIntersection(insidePoints[1], outsidePoints[0]);
                }

            } else {
                trianglesAreParallel = true;
                double d = plane.distance(position() + triangle[0]);
                if((d < 0) && (abs(d) < 0.1)) {
                    intersect1 = std::pair<Point4D, double>(position() + triangle[0], -d);
                    intersect2 = std::pair<Point4D, double>(position() + triangle[1], -d);
                    intersect3 = std::pair<Point4D, double>(position() + triangle[2], -d);
                } else
                    continue;
            }

            if(checkPointInsideTriangle(intersect1.first, obj.position(), objTriangle) ||
            checkPointInsideTriangle(intersect2.first, obj.position(), objTriangle) ||
            (trianglesAreParallel && checkPointInsideTriangle(intersect3.first, obj.position(), objTriangle))) {
                if(_debugMode) {
                    triangle.color = sf::Color(0, 255, 0);
                    objTriangle.color = sf::Color(255, 0, 0);
                    //std::cout << intersect2.second << std::endl;
                }

                //p_velocity *= -1;
                //applyAcceleration({0, 0, 0});
                collisions++;

                if(!trianglesAreParallel)
                    translate(norm*(intersect2.second + intersect1.second)*0.5*trianglesNormDot);
                else {
                    translate(norm * intersect1.second);
                    //return true;
                }
            }

        }
    }
    //if(collisions)
    //    translate(translation/collisions);
    return collisions > 0;
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


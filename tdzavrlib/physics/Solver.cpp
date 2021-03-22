//
// Created by Иван Ильин on 10.03.2021.
//

#include "Solver.h"
#include <iostream>

void Solver::solveCollision(RigidBody& obj1, RigidBody& obj2, const CollisionPoint& collision) {
    if(!collision.hasCollision)
        return;

    /*
    Point4D v1_parallel = obj1.velocity().dot(collision.normal);
    Point4D v1_perpendicular = obj1.velocity() - v1_parallel;

    Point4D v2_parallel = obj2.velocity().dot(collision.normal);
    Point4D v2_perpendicular = obj2.velocity() - v2_parallel;

    std::cout << v1_parallel.abs() << " " << v1_perpendicular.abs() << std::endl;

    if(obj1.isCollision()) {
        v1_parallel = (v2_parallel*(2.0*obj2.mass()) + v1_parallel*(obj1.mass() - obj2.mass())) / (obj1.mass() + obj2.mass());
        obj1.setVelocity(v1_parallel + v1_perpendicular);
    }

    if(obj2.isCollision()) {
        v2_parallel = (v1_parallel*(2.0*obj1.mass()) + v2_parallel*(obj2.mass() - obj1.mass())) / (obj1.mass() + obj2.mass());
        obj2.setVelocity(v2_parallel + v2_perpendicular);
    }
    */

    if(obj1.isCollision())
        obj1.setVelocity(0);
    if(obj2.isCollision())
        obj2.setVelocity(0);

    if(obj1.isCollision() && obj2.isCollision()) {
        obj1.translate(-collision.normal * collision.depth/2.0);
        obj2.translate(collision.normal * collision.depth/2.0);
    } else if(obj1.isCollision())
        obj1.translate(-collision.normal * collision.depth);
    else
        obj2.translate(collision.normal * collision.depth);
}


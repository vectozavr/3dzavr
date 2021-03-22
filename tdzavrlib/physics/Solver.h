//
// Created by Иван Ильин on 10.03.2021.
//

#ifndef INC_3DZAVR_SOLVER_H
#define INC_3DZAVR_SOLVER_H

#include "RigidBody.h"

class Solver {
public:
    static void solveCollision(RigidBody& obj1, RigidBody& obj2, const CollisionPoint& collision);
};


#endif //INC_3DZAVR_SOLVER_H

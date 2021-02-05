//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef INC_3DZAVR_RIGIDBODY_H
#define INC_3DZAVR_RIGIDBODY_H

#include "../utils/Point4D.h"

class RigidBody {
private:
    Point4D p_position;
    Point4D p_velocity;
    Point4D p_acceleration;

    Point4D p_angle;
    Point4D p_angularVelocity;
    Point4D p_angularAcceleration;


public:
    RigidBody() = default;
};


#endif //INC_3DZAVR_RIGIDBODY_H

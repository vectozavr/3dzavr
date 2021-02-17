//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef INC_3DZAVR_RIGIDBODY_H
#define INC_3DZAVR_RIGIDBODY_H

#include <vector>
#include "../utils/Point4D.h"
#include "../Triangle.h"

class RigidBody {
private:
    Point4D p_velocity;
    Point4D p_acceleration;

    Point4D p_angularVelocity;
    Point4D p_angularAcceleration;

    bool _collision = false;
    bool _debugMode = false;

    static bool checkPointInsideTriangle(const Point4D& point, const Point4D& position, const Triangle& triangle);

public:
    RigidBody() = default;

    bool checkCollision(RigidBody& obj);

    [[nodiscard]] bool collision() const { return _collision; }
    void setCollision(bool c) { _collision= c; }

    [[nodiscard]] virtual std::vector<Triangle>& triangles() = 0;
    [[nodiscard]] virtual Point4D position() const = 0;
    virtual void translate(const Point4D& dv) = 0;
    virtual void rotate(const Point4D& r) = 0;

    void updatePhysicsState();

    void applyVelocity(const Point4D& velocity);
    void applyAngularVelocity(const Point4D& angularVelocity);

    void applyAcceleration(const Point4D& acceleration);
    void applyAngularAcceleration(const Point4D& angularAcceleration);

    void setDebugMode(bool mode) { _debugMode = mode; }
};


#endif //INC_3DZAVR_RIGIDBODY_H

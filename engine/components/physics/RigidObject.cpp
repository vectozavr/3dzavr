#include <cmath>
#include <utility>

#include <components/physics/RigidObject.h>
#include <utils/Log.h>
#include <Consts.h>
#include <components/physics/EPA.h>


Vec3D RigidObject::findFurthestPoint(const Vec3D &direction) {
    Vec3D maxPoint{0, 0, 0};
    double maxDistance = -std::numeric_limits<double>::max();

    Matrix4x4 invModel = getComponent<TransformMatrix>()->fullInvModel();
    Vec3D modelDir = invModel*direction;

    for(auto & it : _hitBox) {
        double distance = it.dot(modelDir);

        if (distance > maxDistance) {
            maxDistance = distance;
            maxPoint = it;
        }
    }

    Matrix4x4 model = getComponent<TransformMatrix>()->fullModel();
    return Vec3D(model*maxPoint.makePoint4D());
}

void RigidObject::updatePhysicsState(double deltaTime) {
    _lastPosition = getComponent<TransformMatrix>()->fullPosition();
    _inCollision = false;

    getComponent<TransformMatrix>()->translate(_velocity * deltaTime);
    getComponent<TransformMatrix>()->rotateRelativePoint(getComponent<TransformMatrix>()->fullPosition() + _centerOfMass, _angularVelocity * deltaTime);

    _acceleration += _appliedAcceleration;
    _velocity += _acceleration * deltaTime;
    _acceleration = Vec3D(0,0,0);

    _angularVelocity += _angularAcceleration * deltaTime;
    _angularAcceleration = Vec3D(0,0,0);
}

void RigidObject::setVelocity(const Vec3D &velocity) {
    _velocity = velocity;
}

void RigidObject::setAngularVelocity(const Vec3D &angularVelocity) {
    _angularVelocity = angularVelocity;
}

void RigidObject::addVelocity(const Vec3D &velocity) {
    _velocity = _velocity + velocity;
}

void RigidObject::addAngularVelocity(const Vec3D &angularVelocity) {
    _angularVelocity = _angularVelocity + angularVelocity;
}

void RigidObject::setAcceleration(const Vec3D &acceleration) {
    _appliedAcceleration = acceleration;
}

void RigidObject::setRestitution(double restitution) {
    _restitution = restitution;
}

bool RigidObject::initHitBox() {
    auto triangleMeshComponent = getComponent<TriangleMesh>();
    if(triangleMeshComponent) {
        _hitBox = HitBox(*triangleMeshComponent, _useSimpleBox);
        computeCenterOfMass(triangleMeshComponent);
        computeInertiaTensor(triangleMeshComponent);
        return true;
    }
    auto lineMeshComponent = getComponent<LineMesh>();
    if(lineMeshComponent) {
        _hitBox = HitBox(*lineMeshComponent, _useSimpleBox);
        computeCenterOfMass(lineMeshComponent);
        computeInertiaTensor(lineMeshComponent);
        return true;
    }

    return false;
}

void RigidObject::computeCenterOfMass(const std::shared_ptr<TriangleMesh>& triangleMesh) {
    Vec3D weightedCentroidSum(0);
    _volume = 0.0;
    _surfaceArea = 0.0;

    for (const auto& triangle : triangleMesh->triangles()) {
        Vec3D v1(triangle[0]);
        Vec3D v2(triangle[1]);
        Vec3D v3(triangle[2]);

        Vec3D centroid = (v1 + v2 + v3)/3;

        // Assuming the reference point O is the origin (0, 0, 0) for simplicity
        double dVolume = std::abs(v1.dot(v2.cross(v3))/6);

        weightedCentroidSum += centroid * dVolume;
        _volume += dVolume;

        _surfaceArea += std::abs(((v2-v1).cross(v3-v1)).abs()/2);
    }
    _centerOfMass = weightedCentroidSum / _volume;
}

void RigidObject::computeInertiaTensor(const std::shared_ptr<TriangleMesh>& triangleMesh) {
    _inertiaTensor = Matrix3x3::Zero();

    for (const auto& triangle : triangleMesh->triangles()) {
        Vec3D v1(triangle[0]);
        Vec3D v2(triangle[1]);
        Vec3D v3(triangle[2]);

        Vec3D centroid = (v1 + v2 + v3)/3;

        double dVolume = std::abs(v1.dot(v2.cross(v3))/6);

        // Position vector from the origin to the centroid, then to the actual COM of the tetrahedron
        Vec3D r = centroid/4 - _centerOfMass;

        // For uniform density, distribute the total mass according to the volume ratio
        double dMass = _mass * (dVolume / _volume);

        // Here we assume that entire dMass is located in centroid/4
        _inertiaTensor += (Matrix3x3::Identity()*r.sqrAbs() - Matrix3x3::Outer(r, r))*dMass;
    }
    _invInertiaTensor = _inertiaTensor.inverse();
}

void RigidObject::computeCenterOfMass(const std::shared_ptr<LineMesh>& lineMesh) {
    // TODO
}

void RigidObject::computeInertiaTensor(const std::shared_ptr<LineMesh>& lineMesh) {
    // TODO
}

void RigidObject::start() {
    if (!hasComponent<TransformMatrix>()) {
        // This component requires to work with TransformMatrix component,
        addComponent<TransformMatrix>();
    }
    initHitBox();
}

void RigidObject::fixedUpdate(double deltaTime) {
    if(_hitBox.empty()) {
        if(!initHitBox()) {
            return;
        }
    }

    updatePhysicsState(deltaTime);
}

void RigidObject::SolveCollision(const EPA::CollisionPoint &collision,
                                 const std::shared_ptr<RigidObject>& obj1,
                                 const std::shared_ptr<RigidObject>& obj2) {
    // This is the old version without physics simulations
    /*
    Vec3D velocity_perpendicular = collision.normal * obj1->velocity().dot(collision.normal);
    Vec3D velocity_parallel = obj1->velocity() - velocity_perpendicular;
    obj1->setVelocity(velocity_parallel);
    */

    Vec3D pos1 = obj1->getComponent<TransformMatrix>()->fullPosition() + obj1->centerOfMass();
    Vec3D pos2 = obj2->getComponent<TransformMatrix>()->fullPosition() + obj2->centerOfMass();

    /*
    for(const auto& p: collision.collisionPlane.points) {

        Vec3D rA = p - pos1;
        Vec3D rB = p - pos2;

        Vec3D relativeVelocity = obj2->velocity() + obj2->angularVelocity().cross(rB) -
                                    obj1->velocity() - obj1->angularVelocity().cross(rA);

        // TODO: experiment with ks and kd. Maybe they should be different for different materials
        double ks = 10.0;
        double kd = 10.0;

        //Vec3D springForce = collision.normal*collision.depth*ks - collision.normal*collision.normal.dot(relativeVelocity)*kd;
        Vec3D springForce = collision.normal*collision.depth*ks;

        if(obj1->hasCollision()) {
            obj1->_acceleration += -springForce/obj1->mass();
            obj1->_angularAcceleration += obj1->invInertiaTensor()*rA.cross(-springForce);
        }
        if(obj2->hasCollision()) {
            obj2->_acceleration += springForce/obj2->mass();
            obj2->_angularAcceleration += obj2->invInertiaTensor()*rB.cross(springForce);
        }
    }*/


    Vec3D rA = collision.collisionPlane.origin - pos1;
    Vec3D rB = collision.collisionPlane.origin - pos2;
    Vec3D relativeVelocity =
            obj2->velocity() + obj2->angularVelocity().cross(rB) -
            obj1->velocity() - obj1->angularVelocity().cross(rA);

    double velocityAlongNormal = relativeVelocity.dot(collision.normal);

    if (velocityAlongNormal > 0) {
        return; // Objects moving apart
    }

    double e = std::min(obj1->restitution(), obj2->restitution());

    double numerator = -(1.0 + e) * velocityAlongNormal;
    double denominator = 1.0/obj1->mass() + 1.0/obj2->mass() +
            collision.normal.dot(
                    (obj1->invInertiaTensor()*rA.cross(collision.normal)).cross(rA) +
                    (obj2->invInertiaTensor()*rB.cross(collision.normal)).cross(rB));

    double j = numerator / denominator;

    Vec3D impulse = collision.normal*j;


    /*TODO: Maybe we need to add property like _isPhysical here so the
     * object can have the collision, but we do not need to update its physics state
     * Now if we want the object to have collision and not being updated here we can set
     * its mass to a very large number
     */


    if(obj1->hasCollision()) {
        obj1->addVelocity(-impulse / obj1->mass());
        obj1->addAngularVelocity(-(obj1->invInertiaTensor()*rA.cross(impulse)));

        // Just a test
        Vec3D velocity_perpendicular = collision.normal * obj1->velocity().dot(collision.normal);
        Vec3D velocity_parallel = obj1->velocity() - velocity_perpendicular;
        obj1->setVelocity(velocity_parallel);
    }

    if(obj2->hasCollision()) {
        obj2->addVelocity(impulse / obj2->mass());
        obj2->addAngularVelocity(obj2->invInertiaTensor()*rB.cross(impulse));
    }



    // TODO: static and dynamic friction
    /*
    Vec3D tangentialVelocity = relativeVelocity - collision.normal*relativeVelocity.dot(collision.normal);
    if (tangentialVelocity.abs() > Consts::EPS) {
        Vec3D t = tangentialVelocity.normalized();

        //TODO: replace -relativeVelocity.dot(t) with -tangentialVelocity
        double j_t = -relativeVelocity.dot(t) / (1.0/obj1->mass() + 1.0/obj2->mass());

        // Determine the dynamic friction coefficient (average if different for each object)
        double mu_d = (obj1->dynamicFriction() + obj2->dynamicFriction()) / 2.0;
        // Clamp the friction impulse to the maximum static friction
        double maxFriction = (obj1->staticFriction() + obj2->staticFriction()) / 2.0 * j;
        double frictionImpulseMagnitude = std::clamp(j_t, -maxFriction, maxFriction);

        // Apply friction impulse to both objects
        Vec3D frictionImpulse = t * frictionImpulseMagnitude;

        if(obj1->hasCollision()) {
            obj1->addVelocity(-frictionImpulse / obj1->mass());
            obj1->addAngularVelocity(-(obj1->invInertiaTensor()*rA.cross(frictionImpulse)));
        }

        if(obj2->hasCollision()) {
            obj2->addVelocity(frictionImpulse / obj2->mass());
            obj2->addAngularVelocity(obj2->invInertiaTensor()*rB.cross(frictionImpulse));
        }
    }
     */
}

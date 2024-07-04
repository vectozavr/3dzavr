#ifndef PHYSICS_RIGIDBODY_H
#define PHYSICS_RIGIDBODY_H

#include <functional>
#include <utility>
#include <vector>
#include <memory>

#include <components/geometry/TriangleMesh.h>
#include <components/geometry/LineMesh.h>
#include <components/Component.h>
#include <components/physics/EPA.h>
#include <components/physics/HitBox.h>
#include <linalg/Matrix3x3.h>

// TODO: introduce narrow and broad phase collision detection
// TODO: implement simulation of Rigid Body objects: forces, acceleration, momentum (to be able to simulate boxes, balls and etc)
// TODO: implement support of restrictions for Rigid Body objects and connections between several Rigid Body.
// TODO: implement Rag-Doll support for Rigid Body objects (bones, connections, constraints, skeleton and humanoids)

/**
 * @class RigidObject
 * @brief Represents a physical object with rigid body dynamics.
 *
 * This class models a rigid body that can be used in physics simulations. It includes properties such as velocity,
 * acceleration, mass, and others that are necessary for simulating physical behaviors. The class also handles collision
 * detection and response by integrating with the collision detection system (e.g., using EPA for collision resolution).
 * It inherits from the Component class, allowing it to be attached to game objects for simulation within a scene.
 */
class RigidObject : public Component {
private:
    // Physical state variables
    Vec3D _lastPosition; // Stores the last known position of the object.
    Vec3D _velocity{0, 0, 0}; // Current velocity vector of the object.
    Vec3D _acceleration{0, 0, 0}; // Current acceleration vector of the object.
    Vec3D _angularVelocity{0, 0, 0}; // Current angular velocity vector for rotational movement.
    Vec3D _angularAcceleration{0, 0, 0}; // Current angular acceleration vector for rotational movement.
    Vec3D _appliedAcceleration{0,0,0}; // Acceleration applied to the object, excluding gravity or other global forces.

// Physical properties
    double _mass = 1; // Mass of the object. Affects force calculations.
    double _volume = 0; // Volume of the object. Can be used for buoyancy and other volume-dependent calculations.
    double _surfaceArea = 0; // Surface area of the object. Useful for drag and other surface-dependent effects.
    Vec3D _centerOfMass; // Center of mass of the object. Important for rotational dynamics.
    Matrix3x3 _inertiaTensor = Matrix3x3::Zero(); // Inertia tensor of the object, for calculating rotational dynamics.
    Matrix3x3 _invInertiaTensor; // Inverse of the inertia tensor, for optimizing rotational dynamics calculations.
    double _staticFriction = 0.2; // Coefficient of static friction. Used in friction calculations when the object is at rest.
    double _dynamicFriction = 0.15; // Coefficient of dynamic friction. Used in friction calculations when the object is moving.
    double _restitution = 0; // Coefficient of restitution (bounciness). Affects how the object bounces off other objects.

    // Collision detection
    bool _hasCollision = false; // Flag indicating whether the object is currently colliding with another object.

    // Represents the hitbox associated with the rigid object. Used for collision detection and response.
    HitBox _hitBox{};
    // Determines whether a simple (bounding box) or detailed (mesh-based) hitbox is used. Default is true for simple.
    bool _useSimpleBox = true;

    // Indicates if the object is currently involved in a collision. Used to trigger collision-specific behaviors.
    bool _inCollision = false;

    // Callback function for handling collision events. It is called when a collision is detected between this object
    // and another. The function parameters include the collision point and the two colliding objects.
    std::function<void(const EPA::CollisionPoint &,
                       std::shared_ptr<RigidObject>,
                       std::shared_ptr<RigidObject>)> _collisionCallBack;

    /**
     * Initializes the hitbox for the rigid object. This function determines the type of hitbox (simple or detailed)
     * based on the _useSimpleBox flag and generates it accordingly. It must be called after the object's geometry is defined.
     * @return True if the hitbox was successfully initialized, false otherwise.
     */
    bool initHitBox();

    /**
     * Computes the center of mass for the rigid object based on a TriangleMesh. This method is essential for accurately
     * simulating the physics of objects with complex shapes. The center of mass is used in various physics calculations,
     * including inertia and force application.
     * @param triangleMesh A shared pointer to the TriangleMesh representing the object's geometry.
     */
    void computeCenterOfMass(const std::shared_ptr<TriangleMesh>& triangleMesh);

    /**
     * Computes the inertia tensor for the rigid object based on a TriangleMesh. This method is essential for accurately
     * simulating the rotational dynamics of objects with complex shapes. The inertia tensor is used in rotational force
     * calculations and determines how the object rotates in response to applied torques.
     * @param triangleMesh A shared pointer to the TriangleMesh representing the object's geometry.
     */
    void computeInertiaTensor(const std::shared_ptr<TriangleMesh>& triangleMesh);

    /**
     * Computes the center of mass for the rigid object based on a LineMesh. This method is essential for accurately
     * simulating the physics of objects with complex shapes. The center of mass is used in various physics calculations,
     * including inertia and force application.
     * @param lineMesh A shared pointer to the LineMesh representing the object's geometry.
     */
    void computeCenterOfMass(const std::shared_ptr<LineMesh>& lineMesh);

    /**
     * Computes the inertia tensor for the rigid object based on a LineMesh. This method is essential for accurately
     * simulating the rotational dynamics of objects with complex shapes. The inertia tensor is used in rotational force
     * calculations and determines how the object rotates in response to applied torques.
     * @param lineMesh A shared pointer to the LineMesh representing the object's geometry.
     */
    void computeInertiaTensor(const std::shared_ptr<LineMesh>& lineMesh);

public:

    /**
     * Constructs a RigidObject with the specified parameters.
     * @param useSimpleBox A boolean flag indicating whether to use a simple bounding box for collision detection.
     */
    explicit RigidObject(bool useSimpleBox = true) : _useSimpleBox(useSimpleBox) {};
    RigidObject(const RigidObject &rigidBody) = default;

    /**
     * @brief Checks if the object should be checked for collision with another objects.
     * @return True if we have to check, false otherwise.
     */
    [[nodiscard]] bool hasCollision() const { return _hasCollision; }

    /**
     * @brief Checks if the object is currently in collision with another object.
     * @return True if the object is in collision, false otherwise.
     */
    [[nodiscard]] bool inCollision() const { return _inCollision; }

    /**
     * @brief Sets the collision flag for the object.
     * @param c The value to set the collision flag to.
     */
    void setCollision(bool c) { _hasCollision = c; }

    /**
     * @brief Sets the in-collision flag for the object.
     */
    void setInCollision() { _inCollision = true; }

    /**
     * @brief Updates the physics state of the object based on the elapsed time.
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void updatePhysicsState(double deltaTime);

    /**
     * @brief Sets the velocity of the object.
     * @param velocity The new velocity vector to set.
     */
    void setVelocity(const Vec3D &velocity);

    /**
     * @brief Sets the angular velocity of the object.
     * @param angularVelocity The new angular velocity vector to set.
     */
    void setAngularVelocity(const Vec3D &angularVelocity);

    /**
     * @brief Adds to the current velocity of the object.
     * @param velocity The velocity vector to add.
     */
    void addVelocity(const Vec3D &velocity);

    /**
     * @brief Adds to the current angular velocity of the object.
     * @param angularVelocity The angular velocity vector to add.
     */
    void addAngularVelocity(const Vec3D &angularVelocity);

    /**
     * @brief Sets the acceleration of the object.
     * @param acceleration The new acceleration vector to set.
     */
    void setAcceleration(const Vec3D &acceleration);

    /**
     * @brief Sets the restitution (bounciness) of the object.
     * @param restitution The new restitution value to set.
     */
    void setRestitution(double restitution);


    /**
     * @brief Sets the static friction coefficient of the object.
     * @param staticFriction The static friction coefficient to set.
     */
    void setStaticFriction(double staticFriction) { _staticFriction = staticFriction; }

    /**
     * @brief Sets the dynamic friction coefficient of the object.
     * @param dynamicFriction The dynamic friction coefficient to set.
     */
    void setDynamicFriction(double dynamicFriction) { _dynamicFriction = dynamicFriction; }

    /**
     * @brief Sets the mass of the object.
     * @param mass The mass value to set.
     */
    void setMass(double mass) { _mass = mass; }

    /**
     * @brief Retrieves the static friction coefficient of the object.
     * @return The static friction coefficient.
     */
    [[nodiscard]] double staticFriction() const { return _staticFriction; }

    /**
     * @brief Retrieves the dynamic friction coefficient of the object.
     * @return The dynamic friction coefficient.
     */
    [[nodiscard]] double dynamicFriction() const { return _dynamicFriction; }

    /**
     * @brief Retrieves the mass of the object.
     * @return The mass of the object.
     */
    [[nodiscard]] double mass() const { return _mass; }

    /**
     * @brief Retrieves the volume of the object.
     * @return The volume of the object.
     */
    [[nodiscard]] double volume() const { return _volume; }

    /**
     * @brief Retrieves the surface area of the object.
     * @return The surface area of the object.
     */
    [[nodiscard]] double surfaceArea() const { return _surfaceArea; }

    /**
     * @brief Retrieves the center of mass of the object.
     * @return The center of mass of the object.
     */
    [[nodiscard]] Vec3D centerOfMass() const { return _centerOfMass; }

    /**
     * @brief Retrieves the inertia tensor of the object.
     * @return The inertia tensor of the object.
     */
    [[nodiscard]] Matrix3x3 inertiaTensor() const { return _inertiaTensor; }

    /**
     * @brief Retrieves the inverse of the inertia tensor of the object.
     * @return The inverse of the inertia tensor.
     */
    [[nodiscard]] Matrix3x3 invInertiaTensor() const { return _invInertiaTensor; }

    /**
 * @brief Retrieves the size of the hitbox.
 * @return The size of the hitbox.
 */
    [[nodiscard]] size_t hitBoxSize() const { return _hitBox.size(); }

    /**
     * @brief Retrieves the hitbox of the object.
     * @return The hitbox of the object.
     */
    [[nodiscard]] HitBox hitBox() const { return _hitBox; }

    /**
     * @brief Retrieves the current velocity of the object.
     * @return The current velocity of the object.
     */
    [[nodiscard]] Vec3D velocity() const { return _velocity; }

    /**
     * @brief Retrieves the current angular velocity of the object.
     * @return The current angular velocity of the object.
     */
    [[nodiscard]] Vec3D angularVelocity() const { return _angularVelocity; }

    /**
     * @brief Retrieves the current acceleration of the object.
     * @return The current acceleration of the object.
     */
    [[nodiscard]] Vec3D acceleration() const { return _acceleration; }

    /**
     * @brief Retrieves the restitution coefficient of the object.
     * @return The restitution coefficient of the object.
     */
    [[nodiscard]] double restitution() const { return _restitution; }

    /**
     * @brief Retrieves the last known position of the object.
     * @return The last known position of the object.
     */
    [[nodiscard]] Vec3D lastPosition() const { return _lastPosition; }

    /**
     * @brief Finds the furthest point in the given direction using the object's shape.
     * This method is used in collision detection algorithm (GJK).
     * @param direction The direction vector in which to find the furthest point.
     * @return The furthest point in the given direction.
     */
    Vec3D findFurthestPoint(const Vec3D &direction);

    /**
     * @brief Accessor for the collision callback function.
     * This callback function is invoked when a collision is detected between this object and another.
     * @return A constant reference to the collision callback function.
     */
    [[nodiscard]] const std::function<void(const EPA::CollisionPoint &,
            std::shared_ptr<RigidObject>,
            std::shared_ptr<RigidObject>)> &
    collisionCallBack() const { return _collisionCallBack; }

    /**
     * @brief Sets the collision callback function for the object.
     * This function is called when a collision is detected between this object and another.
     * @param f The collision callback function to set.
     */
    void setCollisionCallBack(const std::function<void(const EPA::CollisionPoint &,
            std::shared_ptr<RigidObject>,
            std::shared_ptr<RigidObject>)> &f) { _collisionCallBack = f; };

    /**
     * @brief Creates a copy of this RigidObject.
     * This method is used to duplicate the object, preserving its current state.
     * @return A shared pointer to the new copy of the RigidObject.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<RigidObject>(*this);
    }

    /**
     * @brief Initialization method called when the object is first created.
     * This method can be used to set up initial state or register the object with other systems.
     */
    void start() override;

    /**
     * @brief Updates the physics state of the object with a fixed timestep.
     * This method is called at a fixed interval and is used for physics updates.
     * @param deltaTime The time elapsed since the last fixed update, in seconds.
     */
    void fixedUpdate(double deltaTime) override;

    /**
     * @brief Static method to resolve a collision between two RigidObjects.
     * This method applies the necessary physics responses to both objects involved in the collision.
     * @param collision The collision point and normal vector.
     * @param obj1 The first RigidObject involved in the collision.
     * @param obj2 The second RigidObject involved in the collision.
     */
    static void SolveCollision(const EPA::CollisionPoint &collision,
                               const std::shared_ptr<RigidObject>& obj1,
                               const std::shared_ptr<RigidObject>& obj2);
};


#endif //PHYSICS_RIGIDBODY_H

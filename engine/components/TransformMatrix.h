#ifndef COMPONENTS_TRANSFORMMATRIX_H
#define COMPONENTS_TRANSFORMMATRIX_H

#include <components/Component.h>
#include <linalg/Matrix4x4.h>

/**
 * @class TransformMatrix
 * @brief Manages transformations for a component in a 3D space.
 *
 * This class encapsulates a transformation matrix and provides methods to manipulate
 * the position, rotation, and scale of a component in a 3D environment. It extends the Component class,
 * allowing it to be attached to game objects or other entities that require spatial transformations.
 */
class TransformMatrix : public Component {
private:
    // TODO: implement rotation part by quaternions

    Matrix4x4 _transformMatrix = Matrix4x4::Identity(); ///< The 4x4 matrix representing the component's transformations in 3D space.
    /*
     * Take into account that when you rotate a body,
     * you change '_angle' & '_angleLeftUpLookAt' only for this particular body,
     * not for attached objects! Therefore, during rotations
     * '_angle' & '_angleLeftUpLookAt' stays constant for all attached objects.
     */
    Vec3D _angle{0, 0, 0}; ///< Stores the rotation angles around the X, Y, and Z axes for the component.
    Vec3D _angleLeftUpLookAt{0, 0, 0}; ///< Stores the rotation angles that determine the component's left, up, and look-at directions.

public:
    TransformMatrix() = default;
    TransformMatrix(const TransformMatrix& transformMatrix) = default;

    /**
     * @brief Applies a transformation to the component.
     * @param t The transformation matrix to apply.
     */
    void transform(const Matrix4x4 &t);

    /**
     * @brief Resets all transformations applied to the component.
     */
    void undoTransformations() { transform(invModel()); }

    /**
     * @brief Transforms a relative point by a given transformation matrix.
     * @param point The point to transform.
     * @param transform The transformation matrix to apply.
     */
    void transformRelativePoint(const Vec3D &point, const Matrix4x4 &transform);

    /**
     * @brief Translates the component by a given vector.
     * @param dv The vector to translate by.
     */
    void translate(const Vec3D &dv);

    /**
     * @brief Translates the component to a specific point.
     * @param point The point to translate to.
     */
    void translateToPoint(const Vec3D &point);

    /**
     * @brief Attracts the component towards a point by a specified value.
     * @param point The point to attract towards.
     * @param value The value of attraction.
     */
    void attractToPoint(const Vec3D &point, double value);

    /**
     * @brief Scales the component by a given vector.
     * @param s The vector to scale by.
     */
    void scale(const Vec3D &s);

    /**
     * @brief Scales the component relative to inside coordinate system.
     * @param s The vector to scale by.
     */
    void scaleInside(const Vec3D &s);

    /**
     * @brief Rotates the component by a given vector.
     * @param r The vector to rotate by.
     */
    void rotate(const Vec3D &r);

    /**
     * @brief Rotates the component around a given vector by some value.
     * @param v The vector to rotate around.
     * @param rv The value of rotation.
     */
    void rotate(const Vec3D &v, double rv);

    /**
     * @brief Change the angle of rotation to a specific angle.
     * @param v The angle.
     */
    void rotateToAngle(const Vec3D &v);

    /**
     * @brief Rotates a component relative to a specific point.
     *
     * This method rotates a component in space relative to specific point. The amount of rotation is defined
     * by a vector specifying the rotation angles around each axis.
     *
     * @param s The point in space to rotate around.
     * @param r The rotation angles around the X, Y, and Z axes.
     */
    void rotateRelativePoint(const Vec3D &s, const Vec3D &r);

    /**
     * @brief Rotates a component relative to specific point and axis.
     *
     * This method rotates a component in space relative to to specific point and axis.
     * The axis of rotation is defined by a vector, and the amount of rotation is specified by a single value.
     *
     * @param s The point in space to rotate around.
     * @param v The axis vector around which to rotate.
     * @param r The rotation angle in degrees.
     */
    void rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r);

    /**
     * @brief Rotates the component relative to its current position.
     *
     * This method applies a rotation to the component itself, relative to its current position. The rotation
     * is defined by a vector specifying the rotation angles around the X, Y, and Z axes.
     *
     * @param r The rotation angles around the X, Y, and Z axes.
     */
    void rotateRelativeItself(const Vec3D &r);

    /**
     * @brief Rotates the component relative to its current position around a specific axis by a given angle.
     *
     * This method rotates the component, relative to its current position, around a specific axis.
     * The axis of rotation is defined by a vector, and the amount of rotation is specified by a single angle value.
     *
     * @param v The axis vector around which to rotate.
     * @param r The rotation angle in degrees.
     */
    void rotateRelativeItself(const Vec3D &v, double r);

    /**
     * @brief Rotates the component relative to its first (left direction) basis vector by a specified angle.
     *
     * This method applies a rotation around components first basis vector.
     *
     * @param rl The rotation angle in degrees.
     */
    void rotateLeft(double rl);

    /**
     * @brief Rotates the component relative to its second (up direction) basis vector by a specified angle.
     *
     * This method applies a rotation around components second basis vector.
     *
     * @param ru The rotation angle in degrees.
     */
    void rotateUp(double ru);

    /**
     * @brief Rotates the component relative to its third (look at direction) basis vector by a specified angle.
     *
     * This method applies a rotation around components third basis vector.
     *
     * @param rlAt The rotation angle in degrees.
     */
    void rotateLookAt(double rlAt);

    /**
     * @brief Returns the left direction vector of the component.
     *
     * This method calculates the left direction vector by normalizing the x-axis vector of the component's transformation matrix.
     * It represents the direction to the left of the component in its local coordinate system.
     *
     * @return Vec3D The normalized left direction vector.
     */
    [[nodiscard]] Vec3D left() const { return _transformMatrix.x().normalized(); }

    /**
     * @brief Returns the up direction vector of the component.
     *
     * This method calculates the up direction vector by normalizing the y-axis vector of the component's transformation matrix.
     * It represents the direction above the component in its local coordinate system.
     *
     * @return Vec3D The normalized up direction vector.
     */
    [[nodiscard]] Vec3D up() const { return _transformMatrix.y().normalized(); }

    /**
     * @brief Returns the look-at direction vector of the component.
     *
     * This method calculates the look-at direction vector by normalizing the z-axis vector of the component's transformation matrix.
     * It represents the direction the component is facing in its local coordinate system.
     *
     * @return Vec3D The normalized look-at direction vector.
     */
    [[nodiscard]] Vec3D lookAt() const { return _transformMatrix.z().normalized(); }

    /**
     * @brief Returns the position vector of the component.
     *
     * This method retrieves the position vector from the w-axis of the component's transformation matrix.
     * It represents the current position of the component in the world space.
     *
     * @return Vec3D The position vector.
     */
    [[nodiscard]] Vec3D position() const { return _transformMatrix.w(); }

    /**
     * @brief Returns the full position vector of the component, considering all parent transformations.
     *
     * This method calculates the full position vector by retrieving the w-axis of the full model matrix,
     * which includes all transformations applied to the component and its parents.
     *
     * @return Vec3D The full position vector in world space.
     */
    [[nodiscard]] Vec3D fullPosition() const { return fullModel().w(); }

    /**
     * @brief Returns the rotation angles of the component.
     *
     * This method retrieves the rotation angles stored in the component, representing the rotation around the X, Y, and Z axes.
     *
     * @return Vec3D The rotation angles vector.
     */
    [[nodiscard]] Vec3D angle() const { return _angle; }

    /**
     * @brief Returns the rotation angles that determine the component's left, up, and look-at directions.
     *
     * This method retrieves the rotation angles that are used to maintain the orientation of the component during rotations.
     * These angles help in determining the left, up, and look-at directions without affecting attached objects.
     *
     * @return Vec3D The rotation angles vector for left, up, and look-at directions.
     */
    [[nodiscard]] Vec3D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    /**
     * @brief Returns the transformation matrix of this component.
     *
     * This method retrieves the transformation matrix representing the current state of the component
     * in 3D space. It encapsulates all the transformations (translation, rotation, scaling) applied to
     * the component up to this point.
     *
     * @return Matrix4x4 The transformation matrix of the component.
     */
    [[nodiscard]] Matrix4x4 model() const;

    /**
     * @brief Returns the cumulative transformation matrix of this component and its ancestors.
     *
     * This method calculates the full transformation matrix by multiplying the current component's
     * transformation matrix with those of its ancestors, effectively accumulating all transformations
     * applied from the top of the hierarchy down to this component. This is useful for determining the
     * component's final position, rotation, and scale in the world space.
     *
     * @return Matrix4x4 The cumulative transformation matrix of the component and its ancestors.
     */
    [[nodiscard]] Matrix4x4 fullModel() const;

    /**
     * @brief Calculates the inverse of the component's transformation matrix.
     *
     * This method computes the inverse of the component's transformation matrix, which can be used
     * to reverse the transformations applied to the component. If the transformation matrix's columns
     * are perpendicular to each other, a fast inverse calculation method is used. Otherwise, a more
     * computationally intensive full inverse calculation is performed.
     *
     * @return Matrix4x4 The inverse of the component's transformation matrix.
     */
    [[nodiscard]] Matrix4x4 invModel() const {return  Matrix4x4::View(model()); }

    /**
     * @brief Calculates the full inverse of the component's transformation matrix.
     *
     * This method computes the full inverse of the component's transformation matrix, which can be used
     * to reverse all transformations applied to the component and its ancestors. This is useful for
     * determining the component's original position, rotation, and scale in the world space.
     *
     * @return Matrix4x4 The full inverse of the component's transformation matrix.
     */
    [[nodiscard]] Matrix4x4 fullInvModel() const { return Matrix4x4::View(fullModel()); }


    /**
     * @brief Creates a copy of this TransformMatrix component.
     *
     * @return std::shared_ptr<Component> A shared pointer to the newly created copy of the TransformMatrix.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<TransformMatrix>(*this);
    }
};


#endif //COMPONENTS_TRANSFORMMATRIX_H

#ifndef OBJECTS_CAMERA_H
#define OBJECTS_CAMERA_H

#include <vector>

#include <components/geometry/Plane.h>
#include <components/geometry/TriangleMesh.h>
#include <components/geometry/LineMesh.h>

/**
 * @class Camera
 * @brief Represents a camera in a 3D scene.
 *
 * This class encapsulates the functionality of a camera used to view 3D scenes. It inherits from the Object class,
 * allowing it to be managed alongside other objects in the scene. The camera's view is defined by its field of view,
 * aspect ratio, and near and far clipping planes. It also supports projecting 3D geometry onto a 2D plane.
 */
class Camera final : public Object {
private:
    std::vector<Plane> _clipPlanes; ///< Planes used for clipping as part of the camera's view frustum.
    double _znear = 0; ///< The distance to the near clipping plane.
    double _zfar = 0; ///< The distance to the far clipping plane.
    double _fov = 0; ///< The field of view of the camera in degrees.
    bool _ready = false; ///< A flag indicating whether the camera is ready to be used.
    double _aspect = 0; ///< The aspect ratio of the camera's view.


    // Internal variables to reduce allocations
    std::vector<std::pair<Vec3D, Vec3D>> _clipBuffer1; ///< Temporary buffer used during clipping operations.
    std::vector<std::pair<Vec3D, Vec3D>> _clipBuffer2; ///< Another temporary buffer used during clipping operations.

    Matrix4x4 _SP; ///< A matrix representing some internal state or transformation related to the camera.

    std::shared_ptr<TransformMatrix> _transformMatrix; ///< A shared pointer to a TransformMatrix component, representing the camera's position and orientation.

public:
    /**
     * @brief Constructor that initializes the camera with a default TransformMatrix component.
     */
    Camera() : Object(ObjectTag("Camera")) {
        _transformMatrix = addComponent<TransformMatrix>();
    };

    // Copy constructor is deleted to prevent copying of Camera instances.
    Camera(const Camera &camera) = delete;

    /**
     * @brief Initializes the camera with the given parameters.
     *
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     * @param fov The field of view in degrees.
     * @param ZNear The distance to the near clipping plane.
     * @param ZFar The distance to the far clipping plane.
     */
    void init(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 5000.0);

    /**
     * @brief Projects a TriangleMesh onto a 2D plane.
     *
     * @param triangleMesh The TriangleMesh to project.
     * @return A vector of pairs of Triangles (2D projected triangle and its 3D non-projected counterpart), representing the projected mesh.
     */
    std::vector<std::pair<Triangle, Triangle>> project(const TriangleMesh& triangleMesh);

    /**
     * @brief Projects a LineMesh onto a 2D plane.
     *
     * @param lineMesh The LineMesh to project.
     * @return A vector of Lines, representing the projected mesh.
     */
    std::vector<Line> project(const LineMesh& lineMesh);

    /**
     * @brief Returns a shared pointer to the camera's TransformMatrix component.
     *
     * @return A shared pointer to the TransformMatrix component.
     */
    std::shared_ptr<TransformMatrix> transformMatrix() const { return _transformMatrix; }
};


#endif //OBJECTS_CAMERA_H

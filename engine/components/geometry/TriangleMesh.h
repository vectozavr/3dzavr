#ifndef GEOMETRY_MESH_H
#define GEOMETRY_MESH_H

#include <utility>
#include <vector>

#include <components/geometry/Triangle.h>
#include <components/geometry/Bounds.h>
#include <components/TransformMatrix.h>
#include <components/props/Material.h>

/**
 * @class TriangleMesh
 * @brief A mesh component made up of triangles.
 *
 * TriangleMesh is a component that represents a 3D object made up of multiple triangles.
 * It includes functionality for geometry manipulation, material application, and intersection
 * testing with rays. This class also supports transformations such as scaling, rotation, and
 * translation through the application of a transformation matrix.
 */
class TriangleMesh : public Component, public std::enable_shared_from_this<TriangleMesh> {
public:
    /**
     * @struct IntersectionInformation
     * @brief Holds information about an intersection between a ray and the mesh.
     *
     * This structure contains details of a potential intersection between a ray and
     * the TriangleMesh, including the point of intersection, the normal at the intersection,
     * the distance from the ray's origin to the intersection, and a pointer to the intersected
     * TriangleMesh.
     */
    struct IntersectionInformation final {
        Vec3D pointOfIntersection; ///< The point at which the intersection occurs.
        Vec3D normal; ///< The normal vector at the point of intersection.
        double distanceToObject = std::numeric_limits<double>::infinity(); ///< Distance from the ray's origin to the intersection.
        std::shared_ptr<TriangleMesh> triangleMesh = nullptr; ///< Pointer to the intersected TriangleMesh.
        bool intersected = false; ///< Flag indicating whether an intersection occurred.
        Triangle triangle{}; ///< The triangle within the mesh where the intersection occurred.
    };
private:
    std::vector<Triangle> _tris; ///< Container for the triangles composing the mesh.
    std::shared_ptr<Material> _material = Consts::DEFAULT_MATERIAL; ///< Material applied to the mesh.
    Bounds _bounds; ///< Bounding volume of the mesh.

    bool _visible = true; ///< Visibility flag for the mesh.

    void copyTriangles(const TriangleMesh& mesh, bool deepCopy); ///< Copies triangles from another mesh.
    void calculateBounds(); ///< Calculates the bounding volume of the mesh.

public:
    /**
     * @brief Default constructor for TriangleMesh.
     */
    TriangleMesh() = default;

    /**
     * @brief Deleted copy assignment operator to prevent copying of TriangleMesh instances.
     */
    TriangleMesh &operator=(const TriangleMesh &mesh) = delete;

    /**
     * @brief Copy constructor for TriangleMesh.
     * @param mesh The TriangleMesh instance to copy from.
     * @param deepCopy If true, performs a deep copy of the mesh's triangles.
     */
    TriangleMesh(const TriangleMesh &mesh, bool deepCopy = false);

    /**
     * @brief Constructs a TriangleMesh with a list of triangles and an optional material.
     * @param tries The list of triangles to initialize the mesh with.
     * @param material The material to apply to the mesh. Defaults to the default material.
     */
    explicit TriangleMesh(const std::vector<Triangle> &tries, const std::shared_ptr<Material>& material = Consts::DEFAULT_MATERIAL);

    /**
     * @brief Returns a constant reference to the mesh's triangles.
     * @return A constant reference to the vector of triangles.
     */
    [[nodiscard]] std::vector<Triangle> const &triangles() const { return _tris; }

    /**
     * @brief Applies a transformation matrix to the mesh.
     * @param matrix4X4 The transformation matrix to apply.
     * @return A reference to the transformed TriangleMesh.
     */
    TriangleMesh &operator*=(const Matrix4x4 &matrix4X4);

    /**
     * @brief Sets the mesh's triangles using an rvalue reference to a vector of triangles.
     * @param t The vector of triangles to set.
     */
    void setTriangles(std::vector<Triangle>&& t);

    /**
     * @brief Sets the mesh's triangles using a constant reference to a vector of triangles.
     * @param t The vector of triangles to set.
     */
    void setTriangles(const std::vector<Triangle>& t);

    /**
     * @brief Returns the total number of vertices in the mesh.
     * @return The total number of vertices.
     */
    [[nodiscard]] size_t size() const { return _tris.size() * 3; }

    /**
     * @brief Returns a shared pointer to the mesh's material.
     * @return A shared pointer to the Material instance.
     */
    [[nodiscard]] std::shared_ptr<Material> getMaterial() const { return _material; }

    /**
     * @brief Sets the material of the mesh.
     * @param material A shared pointer to the new Material instance.
     */
    void setMaterial(std::shared_ptr<Material> material) { _material = std::move(material); }

    /**
     * @brief Returns a constant reference to the mesh's bounding volume.
     * @return A constant reference to the Bounds instance.
     */
    [[nodiscard]] const Bounds& bounds() const { return _bounds; }

    /**
     * @brief Sets the visibility of the mesh.
     *
     * This method allows the visibility of the mesh to be toggled. When set to false, the mesh
     * will not be rendered or considered in any visual computations.
     *
     * @param visibility The desired visibility state of the mesh (true for visible, false for invisible).
     */
    void setVisible(bool visibility) { _visible = visibility; }

    /**
     * @brief Checks if the mesh is currently visible.
     *
     * This method returns the current visibility state of the mesh. It can be used to check
     * if the mesh is set to be rendered in the scene.
     *
     * @return True if the mesh is visible, false otherwise.
     */
    [[nodiscard]] bool isVisible() const { return _visible; }

    /**
     * @brief Calculates the intersection information between a ray and the mesh.
     *
     * This method computes whether a ray, defined by a starting point and an end point, intersects
     * with any of the triangles in the mesh. If an intersection occurs, it returns detailed information
     * about the intersection, including the point of intersection, the normal at that point, the distance
     * from the ray's origin, and a pointer to the intersected mesh.
     *
     * @param from The starting point of the ray.
     * @param to The end point of the ray, defining its direction.
     * @return IntersectionInformation containing details of the intersection, if any.
     */
    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to);

    /**
     * @brief Creates a surface mesh.
     *
     * This static method generates a rectangular surface mesh of the specified width and height.
     * The surface is centered at the origin and aligned with the XZ plane. A material can be optionally
     * specified for the surface.
     *
     * @param width The width of the surface.
     * @param height The height of the surface.
     * @param material Optional material to apply to the surface. Defaults to nullptr.
     * @return A TriangleMesh object representing the surface.
     */
    TriangleMesh static Surface(double width, double height, const std::shared_ptr<Material>& material = nullptr);

    /**
     * @brief Creates a cube mesh.
     *
     * This static method generates a cube mesh with sides of the specified size. The cube is centered
     * at the origin. This method is useful for quickly generating simple geometric shapes.
     *
     * @param size The length of each side of the cube.
     * @return A TriangleMesh object representing the cube.
     */
    TriangleMesh static Cube(double size = 1.0);

    /**
     * @brief Creates a line mesh.
     *
     * This static method generates a mesh representing a line segment between two points. The width
     * of the line can be specified. This is useful for visualizing directions or connections between points.
     *
     * @param from The starting point of the line.
     * @param to The end point of the line.
     * @param line_width The width of the line. Defaults to 0.1.
     * @return A TriangleMesh object representing the line.
     */
    TriangleMesh static LineTo(const Vec3D &from, const Vec3D &to, double line_width = 0.1);

    /**
     * @brief Creates an arrow mesh pointing from one point to another.
     *
     * This static method generates a mesh in the shape of an arrow starting at the specified 'from' point
     * and pointing towards the 'to' point. The width of the arrow's body can be specified. This is useful
     * for visualizing directions or forces in a 3D space.
     *
     * @param from The starting point of the arrow.
     * @param to The end point of the arrow, where the arrowhead points to.
     * @param line_width The width of the arrow's body. Defaults to 1.
     * @return A TriangleMesh object representing the arrow.
     */
    TriangleMesh static ArrowTo(const Vec3D& from, const Vec3D& to, double line_width = 1);

    /**
     * @brief Creates a plane mesh based on a normal vector and a point on the plane.
     *
     * This static method generates a mesh representing a plane defined by a normal vector and a point
     * on the plane. The size of the plane can be specified. This is useful for creating flat surfaces
     * in 3D space that are aligned based on the provided normal vector.
     *
     * @param normal The normal vector of the plane.
     * @param point A point on the plane.
     * @param size The size of the plane. Defaults to 3.0.
     * @return A TriangleMesh object representing the plane.
     */
    TriangleMesh static Plane(const Vec3D& normal, const Vec3D& point, double size = 3.0);

    /**
     * @brief Creates a plane mesh based on three points on the plane.
     *
     * This static method generates a mesh representing a plane defined by three points. The size of the
     * plane can be specified. This method is useful for creating planes that pass through specific points
     * in 3D space.
     *
     * @param p1 The first point on the plane.
     * @param p2 The second point on the plane.
     * @param p3 The third point on the plane.
     * @param size The size of the plane. Defaults to 3.0.
     * @return A TriangleMesh object representing the plane.
     */
    TriangleMesh static Plane(const Vec3D& p1, const Vec3D& p2, const Vec3D& p3, double size = 3.0);

    /**
     * @brief Creates a copy of this TriangleMesh instance.
     *
     * This method overrides the virtual copy method from the Component class. It creates a new instance
     * of TriangleMesh that is a copy of this instance, using the copy constructor.
     *
     * @return A shared pointer to the new TriangleMesh instance.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<TriangleMesh>(*this);
    }

    /**
     * @brief Initializes the TriangleMesh component.
     *
     * This method is intended to be overridden by derived classes to perform initialization tasks.
     * It is called when the TriangleMesh component is added to a scene or otherwise needs to be initialized.
     */
    void start() override;
};

#endif //GEOMETRY_MESH_H

#ifndef GEOMETRY_LINEMESH_H
#define GEOMETRY_LINEMESH_H

#include <components/TransformMatrix.h>
#include <components/props/Color.h>
#include <components/geometry/Bounds.h>
#include <components/geometry/Line.h>

/**
 * @class LineMesh
 * @brief Represents a mesh composed of lines, including visual properties like color and visibility.
 *
 * This class extends the Component class to represent a mesh that is composed of multiple lines.
 * It includes functionalities to manipulate these lines, change their visual properties, and
 * calculate the mesh's bounding box.
 */
class LineMesh : public Component {
private:
    std::vector<Line> _lines; ///< Collection of lines that make up the mesh.
    Color _color; ///< Color of the mesh.
    Bounds _bounds; ///< Bounding box of the mesh, calculated based on the lines.

    bool _visible = true; ///< Visibility flag for the mesh.

    /**
     * @brief Copies lines from another LineMesh instance.
     * @param lineMesh The LineMesh to copy lines from.
     * @param deepCopy Determines whether a deep copy of the lines should be performed.
     */
    void copyLines(const LineMesh& lineMesh, bool deepCopy);

    /**
     * @brief Calculates the bounding box of the mesh based on its lines.
     */
    void calculateBounds();

public:
    /**
     * @brief Default constructor.
     */
    LineMesh() = default;

    /**
     * @brief Deleted copy assignment operator to prevent copying.
     */
    LineMesh &operator=(const LineMesh &lineMesh) = delete;

    /**
     * @brief Copy constructor with an option for deep copying.
     * @param lineMesh The LineMesh instance to copy from.
     * @param deepCopy If true, performs a deep copy of the lines.
     */
    LineMesh(const LineMesh &lineMesh, bool deepCopy = false);

    /**
     * @brief Constructs a LineMesh with a specified set of lines and an optional color.
     * @param lines Vector of Line objects to initialize the mesh with.
     * @param color Optional color for the mesh, defaults to black.
     */
    explicit LineMesh(const std::vector<Line> &lines, const Color& color = Color::BLACK);

    /**
     * @brief Returns a constant reference to the vector of lines.
     * @return Constant reference to the vector of Line objects.
     */
    [[nodiscard]] std::vector<Line> const &lines() const { return _lines; }

    /**
     * @brief Applies a transformation matrix to all lines in the mesh.
     * @param matrix4X4 The transformation matrix to apply.
     * @return Reference to the transformed LineMesh instance.
     */
    LineMesh &operator*=(const Matrix4x4 &matrix4X4);

    /**
     * @brief Sets the lines of the mesh.
     * @param lines Vector of Line objects to set.
     */
    void setLines(std::vector<Line>&& lines);

    /**
     * @brief Returns the total number of points in the mesh.
     * @return Size of the mesh in terms of the number of points.
     */
    [[nodiscard]] size_t size() const { return _lines.size() * 2; }

    /**
     * @brief Gets the color of the mesh.
     * @return The Color of the mesh.
     */
    [[nodiscard]] Color getColor() const { return _color; }

    /**
     * @brief Sets the color of the mesh.
     * @param color The new color to apply to the mesh.
     */
    void setColor(const Color& color) { _color = color; }

    /**
     * @brief Retrieves the bounds of the mesh.
     * @return A constant reference to the Bounds of the mesh.
     */
    [[nodiscard]] const Bounds& bounds() const { return _bounds; }

    /**
     * @brief Sets the visibility of the mesh.
     * @param visibility The visibility state to set (true for visible, false for not visible).
     */
    void setVisible(bool visibility) { _visible = visibility; }

    /**
     * @brief Checks if the mesh is visible.
     * @return True if the mesh is visible, false otherwise.
     */
    [[nodiscard]] bool isVisible() const { return _visible; }

    /**
     * @brief Creates a cube-shaped LineMesh.
     * @param size The size of the cube.
     * @return A LineMesh object shaped as a cube.
     */
    LineMesh static Cube(double size = 1.0);

    /**
     * @brief Creates a LineMesh representing the frame of the given bounds.
     * @param bounds The bounds to create a frame for.
     * @return A LineMesh object representing the bounds frame.
     */
    LineMesh static BoundsFrame(const Bounds& bounds);

    /**
     * @brief Creates a copy of this LineMesh.
     * @return A shared pointer to a new LineMesh instance that is a copy of this one.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<LineMesh>(*this);
    }

    /**
     * @brief Initializes the LineMesh component.
     */
    void start() override;
};


#endif //INC_3DZAVR_LINEMESH_H

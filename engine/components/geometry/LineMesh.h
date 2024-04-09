#ifndef GEOMETRY_LINEMESH_H
#define GEOMETRY_LINEMESH_H

#include <components/TransformMatrix.h>
#include <components/props/Color.h>
#include <components/geometry/Bounds.h>
#include <components/geometry/Line.h>

class LineMesh : public Component {
private:
    std::vector<Line> _lines;
    Color _color;
    Bounds _bounds;

    bool _visible = true;

    void copyLines(const LineMesh& lineMesh, bool deepCopy);
    void calculateBounds();
public:
    LineMesh() = default;

    LineMesh &operator=(const LineMesh &lineMesh) = delete;
    LineMesh(const LineMesh &lineMesh, bool deepCopy = false);

    explicit LineMesh(const std::vector<Line> &lines, const Color& color = Color::BLACK);

    [[nodiscard]] std::vector<Line> const &lines() const { return _lines; }

    LineMesh &operator*=(const Matrix4x4 &matrix4X4);
    void setLines(std::vector<Line>&& lines);

    [[nodiscard]] size_t size() const { return _lines.size() * 2; }

    [[nodiscard]] Color getColor() const { return _color; }
    void setColor(const Color& color) { _color = color; }

    [[nodiscard]] const Bounds& bounds() const { return _bounds; }

    void setVisible(bool visibility) { _visible = visibility; }

    [[nodiscard]] bool isVisible() const { return _visible; }

    LineMesh static Cube(double size = 1.0);
    LineMesh static BoundsFrame(const Bounds& bounds);

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<LineMesh>(*this);
    }

    void start() override;
};


#endif //INC_3DZAVR_LINEMESH_H

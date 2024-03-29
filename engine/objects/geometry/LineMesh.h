#ifndef GEOMETRY_LINEMESH_H
#define GEOMETRY_LINEMESH_H

#include <objects/Object.h>
#include <objects/props/Color.h>
#include <objects/geometry/Bounds.h>
#include <objects/geometry/Line.h>

class LineMesh : public Object {
private:
    std::vector<Line> _lines;
    Color _color;
    Bounds _bounds;

    bool _visible = true;

    LineMesh &operator*=(const Matrix4x4 &matrix4X4);

    void copyLines(const LineMesh& lineMesh, bool deepCopy);
    void calculateBounds();

public:
    explicit LineMesh(const ObjectTag& nameTag) : Object(nameTag) {};

    LineMesh &operator=(const LineMesh &lineMesh) = delete;
    LineMesh(const LineMesh &lineMesh, bool deepCopy = false);
    LineMesh(const ObjectTag& tag, const LineMesh &lineMesh, bool deepCopy = false);

    explicit LineMesh(const ObjectTag& tag, const std::vector<Line> &lines,
                      const Color& color = Color::BLACK);

    [[nodiscard]] std::vector<Line> const &lines() const { return _lines; }

    void setLines(std::vector<Line>&& lines);

    [[nodiscard]] size_t size() const { return _lines.size() * 2; }

    [[nodiscard]] Color getColor() const { return _color; }
    void setColor(const Color& color) { _color = color; }

    [[nodiscard]] const Bounds& bounds() const { return _bounds; }

    void setVisible(bool visibility) { _visible = visibility; }

    [[nodiscard]] bool isVisible() const { return _visible; }

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<LineMesh>(tag, *this);
    }

    LineMesh static Cube(const ObjectTag &tag, double size = 1.0);
    LineMesh static BoundsFrame(const ObjectTag &tag, const Bounds& bounds);
};


#endif //INC_3DZAVR_LINEMESH_H

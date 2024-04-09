#include "LineMesh.h"

LineMesh &LineMesh::operator*=(const Matrix4x4 &matrix4X4) {
    std::vector<Line> newLines;
    newLines.reserve(_lines.size());
    for (auto &line : _lines) {
        newLines.emplace_back(line*matrix4X4);
    }
    setLines(std::move(newLines));

    return *this;
}

void LineMesh::copyLines(const LineMesh &lineMesh, bool deepCopy) {
    if(deepCopy) {
        _lines.reserve(lineMesh._lines.size());
        for (const auto& tr : lineMesh._lines) {
            _lines.push_back(tr);
        }
    } else {
        _lines = lineMesh._lines;
    }
    _bounds = lineMesh._bounds;
}

void LineMesh::calculateBounds() {
    Vec3D min = _lines.empty() ? Vec3D() : Vec3D(_lines[0].p1());
    Vec3D max = min;
    for (const auto & line : _lines) {
        for (int j = 0; j < 3; j++) {
            min[j] = std::min({min[j], line.p1()[j], line.p2()[j]});
            max[j] = std::max({max[j], line.p1()[j], line.p2()[j]});
        }
    }
    _bounds = Bounds {
            .center = (max + min) / 2,
            .extents = (max - min) / 2
    };
}

LineMesh::LineMesh(const LineMesh &lineMesh, bool deepCopy):
Component(lineMesh), _color(lineMesh._color), _visible(lineMesh._visible) {
    copyLines(lineMesh, deepCopy);
}

LineMesh::LineMesh(const std::vector<Line> &lines,
                   const Color& color): _lines(lines), _color(color) {
    calculateBounds();
}

void LineMesh::setLines(std::vector<Line> &&lines) {
    _lines = std::move(lines);
    calculateBounds();
}

LineMesh LineMesh::Cube(double size) {
    LineMesh cube;

    cube.setLines(std::move(std::vector<Line>{
            {Vec4D{-0.5, -0.5, -0.5, 1}, Vec4D{-0.5, 0.5, -0.5, 1}},
            {Vec4D{-0.5, 0.5, -0.5, 1}, Vec4D{0.5, 0.5, -0.5, 1}},
            {Vec4D{0.5, 0.5, -0.5, 1}, Vec4D{0.5, -0.5, -0.5, 1}},
            {Vec4D{0.5, -0.5, -0.5, 1}, Vec4D{-0.5, -0.5, -0.5, 1}},

            {Vec4D{-0.5, -0.5, -0.5, 1},Vec4D{-0.5, -0.5, 0.5, 1}},
            {Vec4D{-0.5, 0.5, -0.5, 1}, Vec4D{-0.5, 0.5, 0.5, 1}},
            {Vec4D{0.5, 0.5, -0.5, 1}, Vec4D{0.5, 0.5, 0.5, 1}},
            {Vec4D{0.5, -0.5, -0.5, 1}, Vec4D{0.5, -0.5, 0.5, 1}},

            {Vec4D{-0.5, -0.5, 0.5, 1}, Vec4D{-0.5, 0.5, 0.5, 1}},
            {Vec4D{-0.5, 0.5, 0.5, 1}, Vec4D{0.5, 0.5, 0.5, 1}},
            {Vec4D{0.5, 0.5, 0.5, 1}, Vec4D{0.5, -0.5, 0.5, 1}},
            {Vec4D{0.5, -0.5, 0.5, 1}, Vec4D{-0.5, -0.5, 0.5, 1}},
    }));

    return cube *= Matrix4x4::Scale(Vec3D(size, size, size));
}

LineMesh LineMesh::BoundsFrame(const struct Bounds& bounds) {
    LineMesh result = LineMesh::Cube();
    result *= Matrix4x4::Translation(bounds.center)*Matrix4x4::Scale(bounds.extents*2);
    return result;
}

void LineMesh::start() {
    if (!hasComponent<TransformMatrix>()) {
        // This component requires to work with TransformMatrix component,
        addComponent<TransformMatrix>();
    }
}

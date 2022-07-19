//
// Created by Иван Ильин on 13.01.2021.
//

#include <utility>

#include "Mesh.h"
#include "utils/ResourceManager.h"
#include "io/Screen.h"

using namespace std;

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    std::vector<Triangle> newTriangles;
    newTriangles.reserve(_tris.size());
    for (auto &t : _tris) {
        newTriangles.emplace_back(t * matrix4X4);
    }
    setTriangles(std::move(newTriangles));

    return *this;
}

void Mesh::loadObj(const std::string &filename, const Vec3D &scale) {
    _tris.clear();
    auto objects = ResourceManager::loadObjects(filename);
    for (auto &obj : objects) {
        for (auto &tri : obj->triangles()) {
            _tris.push_back(tri);
        }
    }
    this->scale(scale);
}

Mesh::Mesh(ObjectNameTag nameTag, const std::string &filename, const Vec3D &scale) : Object(std::move(nameTag)) {
    loadObj(filename, scale);
}

Mesh::Mesh(ObjectNameTag nameTag, const vector<Triangle> &tries) : Object(std::move(nameTag)), _tris(tries) {}

void Mesh::setColor(const sf::Color &c) {
    _color = c;

    for (auto &t : _tris) {
        t.setColor(c);
    }

    // because we change the color of mesh we should update geometry with a new color
    glFreeFloatArray();
}

Mesh
Mesh::LineTo(ObjectNameTag nameTag, const Vec3D &from, const Vec3D &to, double line_width, const sf::Color &color) {

    Mesh line(std::move(nameTag));

    Vec3D v1 = (to - from).normalized();
    Vec3D v2 = from.cross(from + Vec3D{1, 0, 0}).normalized();
    Vec3D v3 = v1.cross(v2).normalized();

    // from plane
    Vec4D p1 = (- v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    Vec4D p2 = (- v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p3 = (  v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p4 = (  v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    // to plane
    Vec4D p5 = (to - from - v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    Vec4D p6 = (to - from - v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p7 = (to - from + v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p8 = (to - from + v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();


    line._tris = std::move(std::vector<Triangle>{
            {p2, p4, p1},
            {p2, p3, p4},
            {p1, p6, p2},
            {p1, p5, p6},
            {p2, p6, p7},
            {p2, p7, p3},
            {p6, p5, p8},
            {p6, p8, p7},
            {p4, p3, p7},
            {p4, p7, p8},
            {p1, p8, p5},
            {p1, p4, p8}
    });
    line.setColor(color);
    line.translateToPoint(from);

    return line;
}


Mesh Mesh::ArrowTo(ObjectNameTag nameTag, const Vec3D &from, const Vec3D &to, double line_width, sf::Color color) {

    Mesh arrow(std::move(nameTag));

    Vec3D v1 = (to - from).normalized();
    Vec3D v2 = from.cross(from + Vec3D{1, 0, 0}).normalized();
    Vec3D v3 = v1.cross(v2).normalized();

    Vec3D to_line = to - v1*0.4;

    // from plane
    Vec4D p1 = (- v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    Vec4D p2 = (- v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p3 = (  v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p4 = (  v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    // to plane
    Vec4D p5 = (to_line - from - v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    Vec4D p6 = (to_line - from - v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p7 = (to_line - from + v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p8 = (to_line - from + v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();

    // arrow
    Vec4D p9  = (to_line - from - v2 * line_width*2 - v3 * line_width*2).makePoint4D();
    Vec4D p10 = (to_line - from - v2 * line_width*2 + v3 * line_width*2).makePoint4D();
    Vec4D p11 = (to_line - from + v2 * line_width*2 + v3 * line_width*2).makePoint4D();
    Vec4D p12 = (to_line - from + v2 * line_width*2 - v3 * line_width*2).makePoint4D();

    Vec4D p13 = (to - from).makePoint4D();

    arrow._tris = std::move(std::vector<Triangle>{
            {p2, p4, p1},
            {p2, p3, p4},
            {p1, p6, p2},
            {p1, p5, p6},
            {p2, p6, p7},
            {p2, p7, p3},
            {p6, p5, p8},
            {p6, p8, p7},
            {p4, p3, p7},
            {p4, p7, p8},
            {p1, p8, p5},
            {p1, p4, p8},

            { p9, p10, p13 },
            { p10, p11, p13 },
            { p11, p12, p13 },
            { p12, p9, p13 },
    });
    arrow.setColor(color);
    arrow.translateToPoint(from);

    return arrow;
}

void Mesh::setOpacity(double t) {
    setColor(sf::Color(_color.r, _color.g, _color.b, t*255));
}

void Mesh::setTriangles(vector<Triangle>&& t) {
    _tris = std::move(t);
}

Mesh::~Mesh() {
    delete[] _geometry;
    _geometry = nullptr;
}

void Mesh::glFreeFloatArray() {
    delete[] _geometry;
    _geometry = nullptr;
}

GLfloat *Mesh::glFloatArray() const {
    if(_geometry != nullptr) {
        return _geometry;
    }
    _geometry = new GLfloat[7 * 3 * _tris.size()];

    for (size_t i = 0; i < _tris.size(); i++) {

        unsigned stride = 21 * i;

        Triangle triangle = _tris[i];
        Vec3D norm = (model()*triangle.norm()).normalized();
        float dot = static_cast<float>(norm.dot(Vec3D(0, 1, 2).normalized()));

        for (int k = 0; k < 3; k++) {
            sf::Color color = triangle.color();
            GLfloat ambientColor[4] = {
                    static_cast<float>(color.r) * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                    static_cast<float>(color.g) * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                    static_cast<float>(color.b) * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                    static_cast<float>(color.a) / 255.0f
            };

            _geometry[stride + 7 * k + 0] = static_cast<GLfloat>(triangle[k].x());
            _geometry[stride + 7 * k + 1] = static_cast<GLfloat>(triangle[k].y());
            _geometry[stride + 7 * k + 2] = static_cast<GLfloat>(triangle[k].z());

            _geometry[stride + 7 * k + 3] = ambientColor[0];
            _geometry[stride + 7 * k + 4] = ambientColor[1];
            _geometry[stride + 7 * k + 5] = ambientColor[2];
            _geometry[stride + 7 * k + 6] = ambientColor[3];
        }
    }

    return _geometry;
}

Mesh Mesh::Cube(ObjectNameTag tag, double size, sf::Color color) {
    Mesh cube(std::move(tag));

    cube._tris = {
            { Vec4D{0.0, 0.0, 0.0, 1.0},    Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0} },
            { Vec4D{0.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 0.0, 0.0, 1.0} },
            { Vec4D{1.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0} },
            { Vec4D{1.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0},    Vec4D{1.0, 0.0, 1.0, 1.0} },
            { Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0} },
            { Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 1.0, 1.0} },
            { Vec4D{0.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 0.0, 1.0} },
            { Vec4D{0.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{0.0, 0.0, 0.0, 1.0} },
            { Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0} },
            { Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0} },
            { Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 0.0, 1.0} },
            { Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 0.0, 0.0, 1.0} },
    };
    cube.setColor(color);

    return cube *= Matrix4x4::Scale(Vec3D(size, size, size))*Matrix4x4::Translation(Vec3D(-0.5, -0.5, -0.5));
}

//
// Created by Иван Ильин on 13.01.2021.
//

#include <utility>

#include "Mesh.h"
#include "utils/ResourceManager.h"
#include "Plane.h"

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    std::vector<Triangle> newTriangles;
    newTriangles.reserve(_tris.size());
    for (auto &t : _tris) {
        newTriangles.emplace_back(t * matrix4X4);
    }
    setTriangles(std::move(newTriangles));

    return *this;
}

Mesh::Mesh(const ObjectTag& tag, const std::vector<Triangle> &tries, std::shared_ptr<Material> material) :
Object(tag), _tris(tries), _material(material) {}

Mesh Mesh::Surface(const ObjectTag &tag, double w, double h, std::shared_ptr<Material> material) {
    Mesh surface(tag);

    surface._tris = {
            { {Vec4D{w/2, 0.0, h/2, 1.0}, Vec4D{w/2, 0.0, -h/2, 1.0}, Vec4D{-w/2, 0.0, -h/2, 1.0}}, {Vec3D{0, 0, 1}, Vec3D{0, 1, 1}, Vec3D{1, 1, 1}} },
            { {Vec4D{-w/2, 0.0, -h/2, 1.0}, Vec4D{-w/2, 0.0, h/2, 1.0}, Vec4D{w/2, 0.0, h/2, 1.0}}, {Vec3D{1, 1, 1}, Vec3D{1, 0, 1}, Vec3D{0, 0, 1}} }
    };
    if(material) {
        surface.setMaterial(material);
    }

    return surface;
}

Mesh Mesh::Cube(const ObjectTag &tag, double size) {
    Mesh cube(tag);

    cube._tris = {
            { {Vec4D{0.0, 0.0, 0.0, 1.0},    Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0}} },
            { {Vec4D{0.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 0.0, 0.0, 1.0}} },
            { {Vec4D{1.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0}} },
            { {Vec4D{1.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0},    Vec4D{1.0, 0.0, 1.0, 1.0}} },
            { {Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0}} },
            { {Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 1.0, 1.0}} },
            { {Vec4D{0.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 0.0, 1.0}} },
            { {Vec4D{0.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{0.0, 0.0, 0.0, 1.0}} },
            { {Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{0.0, 1.0, 1.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0}} },
            { {Vec4D{0.0, 1.0, 0.0, 1.0},    Vec4D{1.0, 1.0, 1.0, 1.0},    Vec4D{1.0, 1.0, 0.0, 1.0}} },
            { {Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 0.0, 1.0}} },
            { {Vec4D{1.0, 0.0, 1.0, 1.0},    Vec4D{0.0, 0.0, 0.0, 1.0},    Vec4D{1.0, 0.0, 0.0, 1.0}} },
    };

    return cube *= Matrix4x4::Scale(Vec3D(size, size, size))*Matrix4x4::Translation(Vec3D(-0.5, -0.5, -0.5));
}

Mesh Mesh::LineTo(const ObjectTag &tag, const Vec3D &from, const Vec3D &to, double line_width) {

    Mesh line(tag);

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
            {{p2, p4, p1}},
            {{p2, p3, p4}},
            {{p1, p6, p2}},
            {{p1, p5, p6}},
            {{p2, p6, p7}},
            {{p2, p7, p3}},
            {{p6, p5, p8}},
            {{p6, p8, p7}},
            {{p4, p3, p7}},
            {{p4, p7, p8}},
            {{p1, p8, p5}},
            {{p1, p4, p8}}
    });
    line.translateToPoint(from);

    return line;
}


Mesh Mesh::ArrowTo(const ObjectTag &tag, const Vec3D &from, const Vec3D &to, double line_width) {

    Mesh arrow(tag);

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
            {{p2, p4, p1}},
            {{p2, p3, p4}},
            {{p1, p6, p2}},
            {{p1, p5, p6}},
            {{p2, p6, p7}},
            {{p2, p7, p3}},
            {{p6, p5, p8}},
            {{p6, p8, p7}},
            {{p4, p3, p7}},
            {{p4, p7, p8}},
            {{p1, p8, p5}},
            {{p1, p4, p8}},

            {{ p9, p10, p13  }},
            {{ p10, p11, p13 }},
            {{ p11, p12, p13 }},
            {{ p12, p9, p13  }},
    });
    arrow.translateToPoint(from);

    return arrow;
}

void Mesh::setTriangles(std::vector<Triangle>&& t) {
    _tris = std::move(t);
}

Mesh::~Mesh() {
    _tris.clear();
}

Object::IntersectionInformation Mesh::intersect(const Vec3D &from, const Vec3D &to) {

    bool intersected = false;
    Vec3D point{};
    double minDistance = std::numeric_limits<double>::infinity();
    Vec3D norm;
    Color color;
    Triangle triangle;

    Matrix4x4 model = this->fullModel();
    // It is computationally more efficient not to transform all object's triangles from model to global
    // coordinate system, but translate 'from' and 'to' vectors inside once and check triangles without performing
    // many matrix multiplication.
    Matrix4x4 invModel = this->fullInvModel();

    Vec3D v = (to - from).normalized();
    Vec3D v_model = Vec3D(invModel*v.makePoint4D());
    Vec3D from_model = Vec3D(invModel*from.makePoint4D());
    Vec3D to_model = Vec3D(invModel*to.makePoint4D());


    for (auto &tri : triangles()) {

        if(tri.norm().dot(v_model) > 0) {
            continue;
        }

        auto trianglePlane = std::make_shared<Plane>(tri, ObjectTag(""));
        auto intersection = trianglePlane->intersect(from_model, to_model);

        if (intersection.distanceToObject > 0 && tri.isPointInside(intersection.pointOfIntersection)) {

            // When you change to model coordinate system you also will get distance scaled by invModel.
            // Due-to this effect if you scale some object in x times you will get distance in x times smaller.
            // That's why we need to perform distance calculation in the global coordinate system where metric
            // is the same for all objects.
            trianglePlane = std::make_shared<Plane>(tri*model, ObjectTag(""));
            auto globalIntersection = trianglePlane->intersect(from, to);
            double globalDistance = (globalIntersection.pointOfIntersection - from).abs();

            if(globalDistance < minDistance) {
                minDistance = globalDistance;
                point = globalIntersection.pointOfIntersection;
                intersected = true;
                norm = Vec3D(model * tri.norm());
                triangle = Triangle({model * tri[0], model * tri[1], model * tri[2]}, tri.textureCoordinates());
            }
        }
    }

    double k = (point - from).abs()/(to-from).abs();

    return IntersectionInformation{point,
                                   norm,
                                   minDistance,
                                   name(),
                                   shared_from_this(),
                                   intersected,
                                   k,
                                   Color{},
                                   triangle};
}

Mesh::Mesh(const Mesh &mesh) :
Object(mesh), _material(mesh._material), _visible(mesh._visible) {
    for (const auto& tr : mesh._tris) {
        _tris.push_back(tr);
    }
}

Mesh::Mesh(const ObjectTag &tag, const Mesh &mesh) :
Object(tag, mesh), _material(mesh._material), _visible(mesh._visible) {
    for (const auto& tr : mesh._tris) {
        _tris.push_back(tr);
    }
}

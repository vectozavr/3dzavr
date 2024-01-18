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

void Mesh::loadObj(const std::string &mesh_file, const std::string &texture_file, const Vec3D &scale) {
    _tris.clear();
    auto objects = ResourceManager::loadObjects(mesh_file, texture_file);
    for (auto &obj : objects) {
        for (auto &tri : obj->triangles()) {
            _tris.push_back(tri);
        }
    }
    _texture = objects[0]->_texture;

    this->scale(scale);
}

Mesh::Mesh(ObjectNameTag nameTag,
           const std::string &mesh_file,
           const std::string &texture_file,
           const Vec3D &scale) : Object(std::move(nameTag)) {
    loadObj(mesh_file, texture_file, scale);
}

Mesh::Mesh(ObjectNameTag nameTag, const std::vector<Triangle> &tries, std::shared_ptr<Texture> texture) :
Object(std::move(nameTag)), _tris(tries), _texture(texture) {}

void Mesh::setColor(const Color &c) {
    _color = c;

    for (auto &t : _tris) {
        t.setColor(c);
    }
}

Mesh Mesh::Surface(ObjectNameTag tag, double w, double h, std::shared_ptr<Texture> texture) {
    Mesh surface(std::move(tag));

    surface._tris = {
            { {Vec4D{w/2, 0.0, h/2, 1.0}, Vec4D{w/2, 0.0, -h/2, 1.0}, Vec4D{-w/2, 0.0, -h/2, 1.0}}, {Vec3D{0, 0, 1}, Vec3D{0, 1, 1}, Vec3D{1, 1, 1}} },
            { {Vec4D{-w/2, 0.0, -h/2, 1.0}, Vec4D{-w/2, 0.0, h/2, 1.0}, Vec4D{w/2, 0.0, h/2, 1.0}}, {Vec3D{1, 1, 1}, Vec3D{1, 0, 1}, Vec3D{0, 0, 1}} }
    };
    if(texture) {
        surface.setTexture(texture);
    }

    return surface;
}

Mesh Mesh::Cube(ObjectNameTag tag, double size, Color color) {
    Mesh cube(std::move(tag));

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
    cube.setColor(color);

    return cube *= Matrix4x4::Scale(Vec3D(size, size, size))*Matrix4x4::Translation(Vec3D(-0.5, -0.5, -0.5));
}

Mesh Mesh::LineTo(ObjectNameTag nameTag, const Vec3D &from, const Vec3D &to, double line_width, const Color &color) {

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
    line.setColor(color);
    line.translateToPoint(from);

    return line;
}


Mesh Mesh::ArrowTo(ObjectNameTag nameTag, const Vec3D &from, const Vec3D &to, double line_width, Color color) {

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
    arrow.setColor(color);
    arrow.translateToPoint(from);

    return arrow;
}

void Mesh::setOpacity(double t) {
    setColor(Color(_color.r(), _color.g(), _color.b(), t*255));
}

void Mesh::setTriangles(std::vector<Triangle>&& t) {
    _tris = std::move(t);
}

Mesh::~Mesh() {

}

Object::IntersectionInformation Mesh::intersect(const Vec3D &from, const Vec3D &to) const {

    bool intersected = false;
    Vec3D point{};
    double minDistance = std::numeric_limits<double>::infinity();
    Vec3D norm;
    Color color;
    Triangle triangle;

    Matrix4x4 model = this->model();
    // It is computationally more efficient not to transform all object's triangles from model to global
    // coordinate system, but translate 'from' and 'to' vectors inside once and check triangles without performing
    // many matrix multiplication.
    Matrix4x4 invModel = this->invModel();

    Vec3D v = (to - from).normalized();
    Vec3D v_model = invModel*v;
    Vec3D from_model = invModel*(from - position());
    Vec3D to_model = invModel*(to - position());


    for (auto &tri : triangles()) {

        if(tri.norm().dot(v_model) > 0) {
            continue;
        }

        auto intersection = Plane(tri, ObjectNameTag("")).intersect(from_model, to_model);

        if (intersection.distanceToObject > 0 && tri.isPointInside(intersection.pointOfIntersection)) {

            // When you change to model coordinate system you also will get distance scaled by invModel.
            // Due-to this effect if you scale some object in x times you will get distance in x times smaller.
            // That's why we need to perform distance calculation in the global coordinate system where metric
            // is the same for all objects.
            Triangle globalTriangle = tri*model;
            auto globalIntersection = Plane(globalTriangle, ObjectNameTag("")).intersect(from, to);
            double globalDistance = (globalIntersection.pointOfIntersection - from).abs();

            if(globalDistance < minDistance) {
                minDistance = globalDistance;
                point = globalIntersection.pointOfIntersection;
                intersected = true;
                norm = Vec3D(model * tri.norm());
                triangle = Triangle({model * tri[0], model * tri[1], model * tri[2]}, tri.textureCoordinates(), tri.colors());
            }
        }
    }

    double kx = (point - from).x()/(to-from).x();
    double ky = (point - from).y()/(to-from).y();

    return IntersectionInformation{point,
                                   norm,
                                   minDistance,
                                   name(),
                                   std::make_shared<Object>(*this),
                                   intersected,
                                   kx,
                                   Color{},
                                   triangle};
}

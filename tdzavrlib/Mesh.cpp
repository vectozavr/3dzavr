//
// Created by Иван Ильин on 13.01.2021.
//

#include <string>
#include <fstream>
#include <strstream>
#include <utility>
#include "Mesh.h"
#include "utils/Log.h"
#include "utils/Time.h"

using namespace std;

Mesh Mesh::operator*(const Matrix4x4 &matrix4X4) const {
    return Mesh(*this) *= matrix4X4;
}

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    for (auto& t : tris)
        t *= matrix4X4;

    return *this;
}

Mesh &Mesh::loadObj(const std::string& filename) {

    ifstream file(filename);

    if (!file.is_open())
    {
        Log::log("Mesh::loadObj(): cannot load mesh from " + filename);
        return *this;
    }

    vector<Point4D> verts;

    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);

        strstream s;
        s << line;

        char junk;
        if (line[0] == 'v')
        {
            Point4D v;
            s >> junk >> v.x >> v.y >> v.z;
            v.w = 1.0;
            verts.push_back(v);
        }
        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris.emplace_back(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] );
        }
    }

    file.close();

    return *this;
}

Mesh::Mesh(const std::string& filename){
    loadObj(filename);
}

Mesh::Mesh(const vector<Triangle> &tries){
    tris = tries;
}

Mesh::Mesh(const Mesh& mesh) : Animatable(mesh) {
    *this = mesh;
}

Mesh Mesh::Obj(const std::string& filename) {
    return Mesh(filename);
}

Mesh Mesh::Cube(double size) {
    Mesh cube{};
    cube.tris = {
            { {0.0, 0.0, 0.0, 1.0},    {0.0, 1.0, 0.0, 1.0},    {1.0, 1.0, 0.0, 1.0} },
            { {0.0, 0.0, 0.0, 1.0},    {1.0, 1.0, 0.0, 1.0},    {1.0, 0.0, 0.0, 1.0} },
            { {1.0, 0.0, 0.0, 1.0},    {1.0, 1.0, 0.0, 1.0},    {1.0, 1.0, 1.0, 1.0} },
            { {1.0, 0.0, 0.0, 1.0},    {1.0, 1.0, 1.0, 1.0},    {1.0, 0.0, 1.0, 1.0} },
            { {1.0, 0.0, 1.0, 1.0},    {1.0, 1.0, 1.0, 1.0},    {0.0, 1.0, 1.0, 1.0} },
            { {1.0, 0.0, 1.0, 1.0},    {0.0, 1.0, 1.0, 1.0},    {0.0, 0.0, 1.0, 1.0} },
            { {0.0, 0.0, 1.0, 1.0},    {0.0, 1.0, 1.0, 1.0},    {0.0, 1.0, 0.0, 1.0} },
            { {0.0, 0.0, 1.0, 1.0},    {0.0, 1.0, 0.0, 1.0},    {0.0, 0.0, 0.0, 1.0} },
            { {0.0, 1.0, 0.0, 1.0},    {0.0, 1.0, 1.0, 1.0},    {1.0, 1.0, 1.0, 1.0} },
            { {0.0, 1.0, 0.0, 1.0},    {1.0, 1.0, 1.0, 1.0},    {1.0, 1.0, 0.0, 1.0} },
            { {1.0, 0.0, 1.0, 1.0},    {0.0, 0.0, 1.0, 1.0},    {0.0, 0.0, 0.0, 1.0} },
            { {1.0, 0.0, 1.0, 1.0},    {0.0, 0.0, 0.0, 1.0},    {1.0, 0.0, 0.0, 1.0} },

            };

    return cube *= Matrix4x4::Scale(size, size, size);
}

void Mesh::translate(double dx, double dy, double dz) {
    p_position += Point4D(dx, dy, dz, 0);
}

void Mesh::rotate(double rx, double ry, double rz) {
    *this *= Matrix4x4::Rotation(rx, ry, rz);
}

void Mesh::rotate(const Point4D &r) {
    *this *= Matrix4x4::Rotation(r);
}

void Mesh::rotate(const Point4D &v, double r) {
    *this *= Matrix4x4::Rotation(v, r);
}

void Mesh::scale(double sx, double sy, double sz) {
    *this *= Matrix4x4::Scale(sx, sy, sz);
}

void Mesh::scale(const Point4D &s) {
    *this *= Matrix4x4::Scale(s.x, s.y, s.z);
}

void Mesh::translate(const Point4D &t) {
    translate(t.x, t.y, t.z);
}

Mesh &Mesh::operator=(const Mesh &mesh) {
    tris = mesh.tris;
    p_position = mesh.p_position;
    c_color = mesh.c_color;
    return *this;
}

void Mesh::rotateRelativePoint(const Point4D &s, double rx, double ry, double rz) {
    // Translate XYZ by vector r1
    Point4D r1 = p_position - s;
    *this *= Matrix4x4::Translation(r1);

    // In translated coordinate system we rotate mesh and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(rx, ry, rz);
    Point4D r2 = rotationMatrix*r1;
    *this *= rotationMatrix;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    *this *= Matrix4x4::Translation(-r2);
    p_position = s + r2;
}

void Mesh::rotateRelativePoint(const Point4D &s, const Point4D &r) {
    rotateRelativePoint(s, r.x, r.y, r.z);
}

void Mesh::rotateRelativePoint(const Point4D &s, const Point4D &v, double r) {
    // Translate XYZ by vector r1
    Point4D r1 = p_position - s;
    *this *= Matrix4x4::Translation(r1);

    // In translated coordinate system we rotate mesh and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, r);
    Point4D r2 = rotationMatrix*r1;
    *this *= rotationMatrix;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    *this *= Matrix4x4::Translation(-r2);
    p_position = s + r2;
}

void Mesh::attractToPoint(const Point4D &point, double r) {
    Point4D v = (point - p_position).normalize();
    translate(v*r);
}

void Mesh::translateToPoint(const Point4D &point) {
    p_position = point;
}

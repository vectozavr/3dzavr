//
// Created by Иван Ильин on 13.01.2021.
//

#include <string>
#include <fstream>
#include <strstream>
#include <utility>
#include "Mesh.h"
#include "Log.h"

using namespace std;

Mesh Mesh::operator*(const Matrix4x4 &matrix4X4) const {
    return Mesh(*this) *= matrix4X4;
}

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    for (auto& t : v_tris)
        t *= matrix4X4;

    return *this;
}

Mesh &Mesh::loadObj(const std::string& filename) {

    ifstream file(filename);

    if (!file.is_open())
    {
        Log::log("Mesh: Cannot load mesh from " + filename);
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
            verts.push_back(v);
        }
        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            v_tris.emplace_back( verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] );
        }
    }

    file.close();

    return *this;
}

Mesh::Mesh(const std::string& filename) {
    loadObj(filename);
}

Mesh Mesh::Obj(const std::string& filename) {
    return Mesh(filename);
}

Mesh Mesh::Cube(double size) {
    // TODO: implement cube
    return Mesh();
}
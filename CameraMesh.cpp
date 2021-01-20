//
// Created by Иван Ильин on 20.01.2021.
//

#include "CameraMesh.h"

CameraMesh::CameraMesh(const Camera& camera) {
    double aspect = (double)camera.width() / (double)camera.height();

    // boarders around the screen
    double z = camera.Zproj();
    triangles.push_back({{aspect, 1, z, 1},
                         {aspect, 1, z, 1},
                         {-aspect, 1, z, 1}});
    triangles.push_back({{-aspect, 1, z, 1},
                         {-aspect, 1, z, 1},
                         {-aspect, -1, z, 1}});

    triangles.push_back({{-aspect, -1, z, 1},
                         {-aspect, -1, z, 1},
                         {aspect, -1, z, 1}});
    triangles.push_back({{aspect, -1, z, 1},
                         {aspect, -1, z, 1},
                         {aspect, 1, z, 1}});

    // Near plane
    double h = camera.Znear() * tan(M_PI*camera.Fov()*0.5/180.0);
    double w = aspect * h;
    z = camera.Znear();
    triangles.push_back({{w, h, z, 1},
                         {w, h, z, 1},
                         {-w, h, z, 1}});
    triangles.push_back({{-w, h, z, 1},
                         {-w, h, z, 1},
                         {-w, -h, z, 1}});

    triangles.push_back({{-w, -h, z, 1},
                         {-w, -h, z, 1},
                         {w, -h, z, 1}});
    triangles.push_back({{w, -h, z, 1},
                         {w, -h, z, 1},
                         {w, h, z, 1}});

    // Far plane
    h = camera.Zfar() * tan(M_PI*camera.Fov()*0.5/180.0);
    w = aspect * h;
    z = camera.Zfar();
    triangles.push_back({{w, h, z, 1},
                         {w, h, z, 1},
                         {-w, h, z, 1}});
    triangles.push_back({{-w, h, z, 1},
                         {-w, h, z, 1},
                         {-w, -h, z, 1}});

    triangles.push_back({{-w, -h, z, 1},
                         {-w, -h, z, 1},
                         {w, -h, z, 1}});
    triangles.push_back({{w, -h, z, 1},
                         {w, -h, z, 1},
                         {w, h, z, 1}});

    // borders
    triangles.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {w, h, z, 1}});
    triangles.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {-w, h, z, 1}});
    triangles.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {w, -h, z, 1}});
    triangles.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {-w, -h, z, 1}});

    for(auto& t : triangles)
        t *= camera.viewInv();
}

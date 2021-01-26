//
// Created by Иван Ильин on 20.01.2021.
//

#include "CameraMesh.h"

CameraMesh::CameraMesh(const Camera& camera) {
    double aspect = (double)camera.width() / (double)camera.height();

    // boarders around the screen
    double z = camera.Zproj();
    tris.push_back({{aspect, 1, z, 1},
                         {aspect, 1, z, 1},
                         {-aspect, 1, z, 1}});
    tris.push_back({{-aspect, 1, z, 1},
                         {-aspect, 1, z, 1},
                         {-aspect, -1, z, 1}});

    tris.push_back({{-aspect, -1, z, 1},
                         {-aspect, -1, z, 1},
                         {aspect, -1, z, 1}});
    tris.push_back({{aspect, -1, z, 1},
                         {aspect, -1, z, 1},
                         {aspect, 1, z, 1}});

    // Near plane
    double h = camera.Znear() * tan(M_PI*camera.Fov()*0.5/180.0);
    double w = aspect * h;
    z = camera.Znear();
    tris.push_back({{w, h, z, 1},
                         {w, h, z, 1},
                         {-w, h, z, 1}});
    tris.push_back({{-w, h, z, 1},
                         {-w, h, z, 1},
                         {-w, -h, z, 1}});

    tris.push_back({{-w, -h, z, 1},
                         {-w, -h, z, 1},
                         {w, -h, z, 1}});
    tris.push_back({{w, -h, z, 1},
                         {w, -h, z, 1},
                         {w, h, z, 1}});

    // Far plane
    h = camera.Zfar() * tan(M_PI*camera.Fov()*0.5/180.0);
    w = aspect * h;
    z = camera.Zfar();
    tris.push_back({{w, h, z, 1},
                         {w, h, z, 1},
                         {-w, h, z, 1}});
    tris.push_back({{-w, h, z, 1},
                         {-w, h, z, 1},
                         {-w, -h, z, 1}});

    tris.push_back({{-w, -h, z, 1},
                         {-w, -h, z, 1},
                         {w, -h, z, 1}});
    tris.push_back({{w, -h, z, 1},
                         {w, -h, z, 1},
                         {w, h, z, 1}});

    // borders
    tris.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {w, h, z, 1}});
    tris.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {-w, h, z, 1}});
    tris.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {w, -h, z, 1}});
    tris.push_back({{0, 0, 0, 1},
                         {0, 0, 0, 1},
                         {-w, -h, z, 1}});

    for(auto& t : tris)
        t *= camera.viewInv();
}

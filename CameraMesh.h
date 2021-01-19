//
// Created by Иван Ильин on 20.01.2021.
//

#ifndef INC_3DZAVR_CAMERAMESH_H
#define INC_3DZAVR_CAMERAMESH_H

#include "Mesh.h"
#include "Camera.h"

class CameraMesh : public Mesh {
public:
    CameraMesh(const Camera& camera);
};


#endif //INC_3DZAVR_CAMERAMESH_H

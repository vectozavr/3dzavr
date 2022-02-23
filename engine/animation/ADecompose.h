//
// Created by Иван Ильин on 23.02.2022.
//

#ifndef INC_3DZAVR_ADECOMPOSE_H
#define INC_3DZAVR_ADECOMPOSE_H

#include "Animation.h"
#include "../Mesh.h"
#include "../Consts.h"

class ADecompose final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;
    std::vector<Triangle> _triangles;

    double _value;
    bool _started = false;

    void update() override {
        auto mesh = _mesh.lock();

        if (mesh == nullptr) {
            stop();
            return;
        }

        if (!_started) {
            _started = true;
            _triangles = _mesh.lock()->triangles();
        }

        std::vector<Triangle> newTriangles;
        newTriangles.reserve(_triangles.size());

        for(auto &t : _triangles) {
            newTriangles.emplace_back((t * Matrix4x4::Translation(t.position().normalized()*progress()*_value)));
        }
        mesh->setTriangles(std::move(newTriangles));
        mesh->glFreeFloatArray();
    }

public:
    ADecompose(std::weak_ptr<Mesh> mesh, double value, double duration = 1, LoopOut looped = LoopOut::None,
               InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                               interpolationType),
                                                                                  _value(value), _mesh(mesh) {}
};

#endif //INC_3DZAVR_ADECOMPOSE_H

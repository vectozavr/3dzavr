#ifndef ANIMATION_ADECOMPOSE_H
#define ANIMATION_ADECOMPOSE_H

#include <animation/Animation.h>
#include <objects/geometry/TriangleMesh.h>
#include <Consts.h>

class ADecompose final : public Animation {
private:
    const std::weak_ptr<TriangleMesh> _mesh;
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
            newTriangles.emplace_back((t * Matrix4x4::Translation(t.centroid().normalized()*progress()*_value)));
        }
        mesh->setTriangles(std::move(newTriangles));
    }

public:
    ADecompose(const std::weak_ptr<TriangleMesh>& triangleMesh, double value, double duration = 1, LoopOut looped = LoopOut::None,
               InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                               interpolationType),
                                                                                  _value(value), _mesh(triangleMesh) {}
};

#endif //ANIMATION_ADECOMPOSE_H

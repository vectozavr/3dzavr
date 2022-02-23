//
// Created by Иван Ильин on 10.11.2021.
//

#ifndef SHOOTER_ASHOWCREATION_H
#define SHOOTER_ASHOWCREATION_H

#include "Animation.h"
#include "../Mesh.h"
#include "../Consts.h"

class AShowCreation final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;
    std::vector<Triangle> _triangles;

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

        double shift = 0.7/_triangles.size();
        double oneTriangleTime = 1.0 - shift*_triangles.size();

        double k = 0.0;
        for(auto &t : _triangles) {
            if(progress() >= shift*k) {

                if(progress() <= shift*k + oneTriangleTime) {
                    double triProgressLinear = (progress() - shift*k) / oneTriangleTime;
                    newTriangles.emplace_back(t[0], t[1], t[1] + (t[2] - t[1]) * triProgressLinear, sf::Color(t.color().r, t.color().g, t.color().b, t.color().a));
                } else {
                    newTriangles.emplace_back(t[0], t[1], t[2], t.color());
                }

            } else {
                newTriangles.emplace_back(t[0], t[0], t[0]);
            }

            k = k + 1.0;
        }
        mesh->setTriangles(std::move(newTriangles));
        mesh->glFreeFloatArray();
    }

public:
    AShowCreation(std::weak_ptr<Mesh> mesh, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh) {}
};

#endif //SHOOTER_ASHOWCREATION_H

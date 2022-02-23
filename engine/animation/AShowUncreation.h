//
// Created by Иван Ильин on 10.11.2021.
//

#ifndef SHOOTER_ASHOWUNCREATION_H
#define SHOOTER_ASHOWUNCREATION_H

#include "Animation.h"
#include "../Mesh.h"

class AShowUncreation final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;
    const std::vector<Triangle> _triangles;

    void update() override {
        auto mesh = _mesh.lock();

        if (mesh == nullptr) {
            stop();
            return;
        }

        std::vector<Triangle> newTriangles;
        newTriangles.reserve(_triangles.size());

        double shift = 0.95/_triangles.size();
        double oneTriangleTime = 1.0 - shift*_triangles.size();

        double k = 0.0;

        double progress_inv = 1 - progress();

        for(auto &t : _triangles) {
            if(progress_inv >= shift*k) {
                if(progress_inv <= shift*k + oneTriangleTime) {
                    double triProgressLinear = (progress_inv - shift*k) / oneTriangleTime;
                    double triProgressBezier = Interpolation::Bezier(Consts::BEZIER[0], Consts::BEZIER[1], triProgressLinear);
                    newTriangles.emplace_back(t[0], t[1], t[1] + (t[2] - t[1]) * triProgressBezier, sf::Color(t.color().r, t.color().g, t.color().b, t.color().a*triProgressBezier));
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
    AShowUncreation(std::weak_ptr<Mesh> mesh, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh), _triangles(mesh.lock()->triangles()) {}
};

#endif //SHOOTER_ASHOWUNCREATION_H

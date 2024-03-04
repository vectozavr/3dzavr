#ifndef ANIMATION_ASHOWCREATION_H
#define ANIMATION_ASHOWCREATION_H

#include <animation/Animation.h>
#include <objects/geometry/Mesh.h>
#include <Consts.h>

class AShowCreation final : public Animation {
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

        // TODO: implement

        double k = 0.0;
        for(auto &t : _triangles) {
            if(progress() >= shift*k) {

                if(progress() <= shift*k + oneTriangleTime) {
                    double triProgressLinear = (progress() - shift*k) / oneTriangleTime;
                    double triProgressBezier = Interpolation::Bezier(Consts::BEZIER[0], Consts::BEZIER[1], triProgressLinear);
                    //newTriangles.emplace_back(t[0], t[1], t[1] + (t[2] - t[1]) * triProgressBezier);
                } else {
                    //newTriangles.emplace_back({{t[0], t[1], t[2]}, {}, {}});
                }

            } else {
                //newTriangles.emplace_back(t[0], t[0], t[0]);
            }

            k = k + 1.0;
        }
        mesh->setTriangles(std::move(newTriangles));
    }

public:
    AShowCreation(std::weak_ptr<Mesh> mesh, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh), _triangles(mesh.lock()->triangles()) {}
};

#endif //ANIMATION_ASHOWCREATION_H

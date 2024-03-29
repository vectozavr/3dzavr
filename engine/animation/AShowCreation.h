#ifndef ANIMATION_ASHOWCREATION_H
#define ANIMATION_ASHOWCREATION_H

#include <animation/Animation.h>
#include <objects/geometry/TriangleMesh.h>
#include <Consts.h>

class AShowCreation final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;
    const std::vector<Triangle> _triangles;
    double _shift; // value from 0 to 1

    void update() override {
        auto mesh = _mesh.lock();

        if (!mesh) {
            stop();
            return;
        }

        std::vector<Triangle> newTriangles;
        newTriangles.reserve(_triangles.size());

        // The time of one triangle
        double dt = 1.0/((_triangles.size()-1)*_shift + 1);
        double k = 0;

        for(auto &t : _triangles) {
            auto& tc = t.textureCoordinates();

            if(progress() >= dt*k*_shift) {
                if(progress() <= dt*(k*_shift + 1)) {
                    double triProgress = (progress() - dt*k*_shift) / dt;

                    newTriangles.emplace_back(Triangle({t[0], t[1], t[1] + (t[2] - t[1]) * triProgress},
                                                       {tc[0], tc[1], tc[1] + (tc[2] - tc[1]) * triProgress}));
                } else {
                    newTriangles.emplace_back(t);
                }
            }

            k += 1;
        }
        mesh->setTriangles(std::move(newTriangles));
    }

public:
    AShowCreation(const std::weak_ptr<Mesh>& mesh, double duration = 1, double shift = 0.005, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh), _triangles(mesh.lock()->triangles()) {
        _shift = std::clamp<double>(shift, 0.0, 1.0);
    }
};

#endif //ANIMATION_ASHOWCREATION_H

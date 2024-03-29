#ifndef ANIMATION_ASHOWUNCREATION_H
#define ANIMATION_ASHOWUNCREATION_H

#include <animation/Animation.h>
#include <objects/geometry/TriangleMesh.h>

class AShowUncreation final : public Animation {
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
        double progress_inv = 1 - progress();

        for(auto &t : _triangles) {
            auto& tc = t.textureCoordinates();

            if(progress_inv >= dt*k*_shift) {
                if(progress_inv <= dt*(k*_shift + 1)) {
                    double triProgress = (progress_inv - dt*k*_shift) / dt;

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
    AShowUncreation(const std::weak_ptr<Mesh>& mesh, double duration = 1, double shift = 0.005, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh), _triangles(mesh.lock()->triangles()) {
        _shift = std::clamp<double>(shift, 0.0, 1.0);
    }
};

#endif //ANIMATION_ASHOWUNCREATION_H

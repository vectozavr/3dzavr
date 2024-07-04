#ifndef ANIMATION_SHOWUNCREATION_H
#define ANIMATION_SHOWUNCREATION_H

#include <animation/Animation.h>
#include <components/geometry/TriangleMesh.h>

/**
 * @class ShowUncreation
 * @brief Animates the deconstruction of a mesh into its constituent triangles.
 *
 * This class is a specialized animation derived from the Animation base class, designed to visually deconstruct
 * a mesh into its individual triangles over time. It achieves this by progressively reducing the size of each
 * triangle in the mesh until they disappear, simulating an "uncreation" effect. This class can be used to create
 * visually appealing transitions or effects in graphical applications where a mesh needs to be removed or transitioned
 * out in a non-instantaneous manner.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _mesh A weak pointer to the TriangleMesh object being animated. This ensures that the animation does not prevent
 *       the mesh object from being destructed and handles the case where the mesh might be deleted before the animation completes.
 * _triangles A vector of Triangle objects representing the initial state of the mesh before the animation starts.
 *            This is used to calculate the progressive "uncreation" effect on each triangle.
 * _shift A value from 0 to 1 indicating the proportion of the animation duration after which each triangle starts
 *        to disappear. A smaller value results in a more staggered effect, while a value closer to 1 makes the triangles
 *        disappear more uniformly.
 *
 * @public_section
 * ShowUncreation Constructor that initializes the animation with the target mesh, duration, shift value, looping behavior,
 *                and interpolation type.
 *                @param triangleMesh A weak pointer to the TriangleMesh object to animate.
 *                @param duration The duration of the animation in seconds.
 *                @param shift The shift value controlling the staggered disappearance of triangles.
 *                @param looped The looping behavior of the animation (None or Continue).
 *                @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies the
 *            "uncreation" effect to the mesh based on the current progress of the animation.
 */
class ShowUncreation final : public Animation {
private:
    const std::weak_ptr<TriangleMesh> _mesh;
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
    ShowUncreation(const std::weak_ptr<TriangleMesh>& triangleMesh, double duration = 1, double shift = 0.005, LoopOut looped = LoopOut::None,
                   InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(triangleMesh), _triangles(triangleMesh.lock()->triangles()) {
        _shift = std::clamp<double>(shift, 0.0, 1.0);
    }
};

#endif //ANIMATION_SHOWUNCREATION_H

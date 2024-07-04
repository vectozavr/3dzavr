#ifndef ANIMATION_SHOWCREATION_H
#define ANIMATION_SHOWCREATION_H

#include <animation/Animation.h>
#include <components/geometry/TriangleMesh.h>
#include <Consts.h>

/**
 * @class ShowCreation
 * @brief Animates the creation of a mesh by progressively revealing its triangles.
 *
 * This class is a specialized animation derived from the Animation base class, designed to visually construct
 * a mesh by progressively revealing its triangles over time. It simulates the effect of a mesh being created
 * or drawn piece by piece, which can be used for visually appealing transitions or introductions of 3D models
 * in graphical applications.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _mesh A weak pointer to the TriangleMesh object being animated. This ensures that the animation does not prevent
 *       the mesh object from being destructed and handles the case where the mesh might be deleted before the animation completes.
 * _triangles A vector of Triangle objects representing the initial state of the mesh before the animation starts.
 *            This is used to calculate the progressive creation effect on each triangle.
 * _shift A value from 0 to 1 indicating the proportion of the animation duration after which each triangle starts
 *        to appear. A smaller value results in a more staggered effect, while a value closer to 1 makes the triangles
 *        appear more uniformly.
 *
 * @public_section
 * ShowCreation Constructor that initializes the animation with the target mesh, duration, shift value, looping behavior,
 *                and interpolation type.
 *                @param triangleMesh A weak pointer to the TriangleMesh object to animate.
 *                @param duration The duration of the animation in seconds.
 *                @param shift The shift value controlling the staggered appearance of triangles.
 *                @param looped The looping behavior of the animation (None or Continue).
 *                @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It calculates and applies the
 *            creation effect to the mesh based on the current progress of the animation.
 */
class ShowCreation final : public Animation {
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
    ShowCreation(const std::weak_ptr<TriangleMesh>& triangleMesh, double duration = 1, double shift = 0.005, LoopOut looped = LoopOut::None,
                 InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(triangleMesh), _triangles(triangleMesh.lock()->triangles()) {
        _shift = std::clamp<double>(shift, 0.0, 1.0);
    }
};

#endif //ANIMATION_SHOWCREATION_H

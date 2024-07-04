#ifndef ANIMATION_DECOMPOSE_H
#define ANIMATION_DECOMPOSE_H

#include <animation/Animation.h>
#include <components/geometry/TriangleMesh.h>
#include <Consts.h>

/**
 * @class Decompose
 * @brief Animates the decomposition of a mesh into its constituent triangles.
 *
 * This class extends the Animation base class to animate the process of decomposing a mesh into its individual
 * triangles. It progressively moves the triangles away from the mesh's centroid, simulating an explosion or
 * decomposition effect. The movement of each triangle is based on the animation's progress and a specified
 * value that controls the speed and direction of the decomposition.
 *
 * @inherit Animation The base class providing the framework for animation timing, progression, and interpolation.
 *
 * @private_section
 * _mesh A weak pointer to the TriangleMesh object being decomposed. This ensures that the animation does not
 *       prevent the mesh from being destructed and handles the case where the mesh might be deleted before
 *       the animation completes.
 * _triangles A vector of Triangle objects representing the current state of the mesh's decomposition.
 * _value A double specifying the speed and direction of the triangles' movement during the decomposition.
 * _started A boolean flag indicating whether the decomposition animation has started. This is used to initialize
 *          the _triangles vector with the mesh's triangles at the start of the animation.
 *
 * @fn update Overrides the pure virtual update function from the Animation class. It updates the position of
 *            each triangle based on the animation's progress and the specified _value, simulating the decomposition.
 *
 * @public_section
 * Decompose Constructor that initializes the animation with the target mesh, value controlling the decomposition,
 *             duration, looping behavior, and interpolation type.
 *             @param triangleMesh A weak pointer to the TriangleMesh object to decompose.
 *             @param value A double specifying the speed and direction of the decomposition.
 *             @param duration The duration of the animation in seconds.
 *             @param looped The looping behavior of the animation (None or Continue).
 *             @param interpolationType The interpolation type for the animation (e.g., Linear, Bezier).
 */
class Decompose final : public Animation {
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
    Decompose(const std::weak_ptr<TriangleMesh>& triangleMesh, double value, double duration = 1, LoopOut looped = LoopOut::None,
              InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                               interpolationType),
                                                                                  _value(value), _mesh(triangleMesh) {}
};

#endif //ANIMATION_DECOMPOSE_H

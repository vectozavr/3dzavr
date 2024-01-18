//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef ENGINE_ACOLOR_H
#define ENGINE_ACOLOR_H

#include <animation/Animation.h>
#include <objects/geometry/Mesh.h>

class AColor final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;

    Color _startColor;
    const Color _newColor;
    bool _started = false;

    void update() override {
        auto mesh = _mesh.lock();

        if (mesh == nullptr) {
            stop();
            return;
        }

        if (!_started) {
            _started = true;
            _startColor = mesh->getColor();
        }

        Vec4D start(_startColor.r(), _startColor.g(), _startColor.b(), _startColor.a());
        Vec4D end(_newColor.r(), _newColor.g(), _newColor.b(), _newColor.a());
        Vec4D midColor = start + (end - start) * progress();

        Color mid(midColor.x(), midColor.y(), midColor.z(), midColor.w());

        mesh->setColor(Color(static_cast<uint8_t>(mid.r()), static_cast<uint8_t>(mid.g()),
                                         static_cast<uint8_t>(mid.b()), static_cast<uint8_t>(mid.a())));
    }

public:
    AColor(std::weak_ptr<Mesh> mesh, const Color &color, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Linear) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh), _newColor(color) {
    }
};

#endif //SHOOTER_3DZAVR_ACOLOR_H

//
// Created by Ivan Ilin on 12/03/2024.
//

#ifndef LIGHTING_LIGHTSOURCE_H
#define LIGHTING_LIGHTSOURCE_H

#include "components/Component.h"

class LightSource : public Component {
protected:
    Color _color = Color::WHITE;
    double _intensity = 1.0;
    std::shared_ptr<TransformMatrix> _transformMatrix = std::make_shared<TransformMatrix>();

public:
    LightSource(const Color& color, double intensity): _color(color), _intensity(std::max(intensity, 0.0)) {}

    LightSource(const LightSource& lightSource) = default;

    [[nodiscard]] inline Color color() const { return _color; };
    [[nodiscard]] inline double intensity() const { return _intensity; };

    /*
     * simplCoef is in range [0, 1], where 0 means exact computation and 1 means simplified.
     * All values between 0 and 1 can be handled in any reasonable way
     * (e.g. linear interpolation between exact and simplified).
     */
    [[nodiscard]] virtual Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const = 0;

    void start() override {
        _transformMatrix = getComponent<TransformMatrix>();
        if (!_transformMatrix) {
            _transformMatrix = assignedToPtr()->addComponent<TransformMatrix>();
        }
    }
};


#endif //LIGHTING_LIGHTSOURCE_H

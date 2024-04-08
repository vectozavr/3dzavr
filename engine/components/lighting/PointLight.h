//
// Created by Ivan Ilin on 12/03/2024.
//

#ifndef LIGHTING_POINTLIGHT_H
#define LIGHTING_POINTLIGHT_H

#include "LightSource.h"

class PointLight final : public LightSource {
public:
    explicit PointLight(const Vec3D& position, const Color& color = Color::WHITE, double intensity = 1.0):
            LightSource(color, intensity) { translate(position); };
    PointLight(const PointLight& pointLight): LightSource(pointLight) { translate(pointLight.position()); };

    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const override {
        auto toLight = fullPosition() - pixelPosition;
        double distance = toLight.abs();
        Vec3D dir = toLight.normalized();

        auto dot = std::clamp<double>(pixelNorm.dot(dir), 0.2, 1);

        // linear interpolation between exact and inexact (with dot = 0.5)
        dot = dot + (0.5 - dot)*simplCoef;

        double energy = intensity()/(distance + 0.1);

        return Color(std::clamp<int>(dot*color().r()*energy, 0, 255),
                     std::clamp<int>(dot*color().g()*energy, 0, 255),
                     std::clamp<int>(dot*color().b()*energy, 0, 255));
    }

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<PointLight>(*this);
    }
};

#endif //LIGHTING_POINTLIGHT_H

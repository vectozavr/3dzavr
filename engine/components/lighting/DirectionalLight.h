#ifndef LIGHTING_DIRECTIONALLIGHT_H
#define LIGHTING_DIRECTIONALLIGHT_H

#include "LightSource.h"

class DirectionalLight final : public LightSource {
private:
    Vec3D _dir;
public:
    explicit DirectionalLight(const Vec3D& direction, const Color& color = Color::WHITE, double intensity = 1.0):
            LightSource(color, intensity), _dir(direction.normalized()) {};
    DirectionalLight(const DirectionalLight& directionalLight) = default;

    [[nodiscard]] inline Vec3D direction() const { return getComponent<TransformMatrix>()->fullModel()*_dir; };

    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const override {
        auto dot = -std::clamp<double>(pixelNorm.dot(direction()), -1, -0.3);

        // linear interpolation between exact and inexact (with dot = 0.5)
        dot = dot + (0.5 - dot)*simplCoef;

        return Color(std::clamp<int>(dot*color().r()*intensity(), 0, 255),
                     std::clamp<int>(dot*color().g()*intensity(), 0, 255),
                     std::clamp<int>(dot*color().b()*intensity(), 0, 255));
    }

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<DirectionalLight>(*this);
    }


};


#endif //LIGHTING_DIRECTIONALLIGHT_H

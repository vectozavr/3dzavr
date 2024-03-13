#ifndef LIGHTING_DIRECTIONALLIGHT_H
#define LIGHTING_DIRECTIONALLIGHT_H

#include <objects/lighting/LightSource.h>

class DirectionalLight final : public LightSource {
private:
    Vec3D _dir;
public:
    DirectionalLight(const ObjectTag& tag, const Vec3D& direction, const Color& color = Color::WHITE, double intensity = 1.0):
            LightSource(tag, color, intensity), _dir(direction.normalized()) {};
    DirectionalLight(const ObjectTag& tag, const DirectionalLight& directionalLight):
            LightSource(tag, directionalLight), _dir(directionalLight._dir) {};

    [[nodiscard]] inline Vec3D direction() const { return _dir; };

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<DirectionalLight>(tag, *this);
    }

    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition) const override {
        auto dot = std::clamp<double>(-pixelNorm.dot(_dir), 0.3, 1);

        return Color(std::clamp<int>(dot*color().r()*intensity(), 0, 255),
                     std::clamp<int>(dot*color().g()*intensity(), 0, 255),
                     std::clamp<int>(dot*color().b()*intensity(), 0, 255));
    }
};


#endif //LIGHTING_DIRECTIONALLIGHT_H

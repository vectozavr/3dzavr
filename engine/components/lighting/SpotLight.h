#ifndef LIGHTING_SPOTLIGHT_H
#define LIGHTING_SPOTLIGHT_H

#include "LightSource.h"

class SpotLight final : public LightSource {
private:
    Vec3D _initialPos;
    Vec3D _dir;
    double _innerConeCos;
    double _outerConeCos;
public:
    SpotLight(const Vec3D& position, const Vec3D& direction, double innerConeCos = cos(Consts::PI / 4),
              double outerConeCos = cos(Consts::PI / 3), const Color& color = Color::WHITE, double intensity = 1.0):
              LightSource(color, intensity), _dir(direction), _innerConeCos(innerConeCos), _outerConeCos(outerConeCos), _initialPos(position) {};
    SpotLight(const SpotLight& spotLight) = default;

    [[nodiscard]] inline Vec3D direction() const { return getComponent<TransformMatrix>()->fullModel()*_dir; };

    [[nodiscard]] double innerConeCos() const { return _innerConeCos; }
    [[nodiscard]] double outerConeCos() const { return _outerConeCos; }
    void setInnerConeCos(double innerConeCos) { _innerConeCos = innerConeCos; }
    void setOuterConeCos(double outerConeCos) { _outerConeCos = outerConeCos; }

    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef = 0.0) const override {
        auto toLight = getComponent<TransformMatrix>()->fullPosition() - pixelPosition;
        double distance = toLight.abs();
        Vec3D dir = toLight.normalized();

        auto dot = std::clamp<double>(pixelNorm.dot(dir), 0.1, 1);

        // linear interpolation between exact and inexact (with dot = 0.5)
        dot = dot + (0.5 - dot)*simplCoef;

        double energy = intensity()/(distance + 0.1);

        double cosAngle = -dir.dot(direction());
        if (cosAngle < _outerConeCos) {
            energy = 0;
        }
        if (cosAngle > _innerConeCos) {
            energy *= (cosAngle - _outerConeCos) / (_innerConeCos - _outerConeCos);
        }

        return Color(std::clamp<int>(dot*color().r()*energy, 0, 255),
                     std::clamp<int>(dot*color().g()*energy, 0, 255),
                     std::clamp<int>(dot*color().b()*energy, 0, 255));
    }

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<SpotLight>(*this);
    }

    void start() override {
        if (!hasComponent<TransformMatrix>()) {
            // This component requires to work with TransformMatrix component,
            addComponent<TransformMatrix>();
        }

        getComponent<TransformMatrix>()->translate(_initialPos);
    }
};

#endif //LIGHTING_SPOTLIGHT_H

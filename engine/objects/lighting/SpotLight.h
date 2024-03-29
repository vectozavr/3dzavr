#ifndef LIGHTING_SPOTLIGHT_H
#define LIGHTING_SPOTLIGHT_H

#include <objects/lighting/LightSource.h>

class SpotLight : public LightSource {
private:
    Vec3D _dir;
    double _innerConeCos;
    double _outerConeCos;
public:
    SpotLight(const ObjectTag& tag, const Vec3D& position, const Vec3D& direction, double innerConeCos = cos(Consts::PI / 4),
              double outerConeCos = cos(Consts::PI / 3), const Color& color = Color::WHITE, double intensity = 1.0):
            LightSource(tag, color, intensity), _dir(direction), _innerConeCos(innerConeCos), _outerConeCos(outerConeCos) { translate(position); };
    SpotLight(const ObjectTag& tag, const SpotLight& spotLight):
            LightSource(tag, spotLight), _dir(spotLight._dir),
            _innerConeCos(spotLight._innerConeCos), _outerConeCos(spotLight._outerConeCos) {
        translate(spotLight.position());
    };

    [[nodiscard]] inline Vec3D direction() const { return fullModel()*_dir; };

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<SpotLight>(tag, *this);
    }

    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef = 0.0) const override {
        auto toLight = fullPosition() - pixelPosition;
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
};

#endif //LIGHTING_SPOTLIGHT_H

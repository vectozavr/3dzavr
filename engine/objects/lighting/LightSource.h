//
// Created by Ivan Ilin on 12/03/2024.
//

#ifndef LIGHTING_LIGHTSOURCE_H
#define LIGHTING_LIGHTSOURCE_H

#include <objects/Object.h>

class LightSource : public Object {
private:
    Color _color = Color::WHITE;
    double _intensity = 1.0;
public:
    LightSource(const ObjectTag& name, const Color& color, double intensity):
    Object(name), _color(color), _intensity(std::max(intensity, 0.0)) {}

    LightSource(const ObjectTag& name, const LightSource& lightSource):
            Object(name), _color(lightSource._color), _intensity(lightSource._intensity) {}

    [[nodiscard]] inline Color color() const { return _color; };
    [[nodiscard]] inline double intensity() const { return _intensity; };

    [[nodiscard]] virtual Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition) const = 0;
};


#endif //LIGHTING_LIGHTSOURCE_H

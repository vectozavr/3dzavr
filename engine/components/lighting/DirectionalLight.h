#ifndef LIGHTING_DIRECTIONALLIGHT_H
#define LIGHTING_DIRECTIONALLIGHT_H

#include "LightSource.h"

/**
 * @class DirectionalLight
 * @brief Represents a directional light source in a scene.
 *
 * Inherits from LightSource and adds functionality specific to directional lighting, such as
 * directionality and illumination based on the angle of incidence. This class is final and cannot
 * be inherited from.
 */
class DirectionalLight final : public LightSource {
private:
    Vec3D _dir; ///< Direction of the light, normalized.

public:
    /**
     * @brief Constructs a DirectionalLight with specified direction, color, and intensity.
     *
     * @param direction The direction vector of the light. It will be normalized upon construction.
     * @param color The color of the light. Defaults to white.
     * @param intensity The intensity of the light. Defaults to 1.0.
     */
    explicit DirectionalLight(const Vec3D& direction, const Color& color = Color::WHITE, double intensity = 1.0):
            LightSource(color, intensity), _dir(direction.normalized()) {};

    /**
     * @brief Default copy constructor.
     *
     * @param directionalLight The DirectionalLight object to copy.
     */
    DirectionalLight(const DirectionalLight& directionalLight) = default;

    /**
     * @brief Gets the direction of the light.
     *
     * This method calculates the direction of the light based on the current transformation matrix,
     * allowing for dynamic changes in direction.
     *
     * @return The calculated direction vector of the light.
     */
    [[nodiscard]] inline Vec3D direction() const { return getComponent<TransformMatrix>()->fullModel()*_dir; };

    /**
     * @brief Calculates the color and intensity of the light as it illuminates a given point.
     *
     * This method takes into account the angle of incidence between the light's direction and the
     * normal at the point being illuminated. It uses linear interpolation to simulate the effect of
     * diffuse reflection based on the simplification coefficient.
     *
     * @param pixelNorm The normal vector at the point being illuminated.
     * @param pixelPosition The position of the point being illuminated (unused in current implementation).
     * @param simplCoef A coefficient used to simplify the calculation, affecting the linear interpolation.
     * @return The color of the light as it would appear on the illuminated point.
     */
    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const override {
        auto dot = -std::clamp<double>(pixelNorm.dot(direction()), -1, -0.3);

        // linear interpolation between exact and inexact (with dot = 0.5)
        dot = dot + (0.5 - dot)*simplCoef;

        return Color(std::clamp<int>(dot*color().r()*intensity(), 0, 255),
                     std::clamp<int>(dot*color().g()*intensity(), 0, 255),
                     std::clamp<int>(dot*color().b()*intensity(), 0, 255));
    }

    /**
     * @brief Creates a copy of this DirectionalLight instance.
     *
     * This method overrides the virtual copy method from the LightSource class. It creates a new instance
     * of DirectionalLight that is a copy of this instance, using the copy constructor. This is useful for
     * duplicating lights with the same properties.
     *
     * @return A shared pointer to the new DirectionalLight instance.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<DirectionalLight>(*this);
    }
};


#endif //LIGHTING_DIRECTIONALLIGHT_H

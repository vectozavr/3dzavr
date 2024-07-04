//
// Created by Ivan Ilin on 12/03/2024.
//

#ifndef LIGHTING_POINTLIGHT_H
#define LIGHTING_POINTLIGHT_H

#include "LightSource.h"

/**
 * @class PointLight
 * @brief A point light source that emits light in all directions from a single point.
 *
 * This class simulates a point light source, which is a common type of light used in 3D rendering.
 * It calculates the illumination of objects based on their distance and angle from the light source,
 * simulating a realistic lighting effect.
 */
class PointLight final : public LightSource {
private:
    Vec3D _initialPos; ///< The initial position of the point light in the scene.
public:
    /**
     * @brief Constructs a PointLight object with specified position, color, and intensity.
     *
     * @param position The initial position of the point light.
     * @param color The color of the light. Defaults to white.
     * @param intensity The intensity of the light. Defaults to 1.0.
     */
    explicit PointLight(const Vec3D& position, const Color& color = Color::WHITE, double intensity = 1.0):
    LightSource(color, intensity), _initialPos(position) {};

    /**
     * @brief Default copy constructor.
     */
    PointLight(const PointLight& pointLight) = default;

    /**
     * @brief Calculates the color and intensity of the light as it illuminates a given point.
     *
     * This method takes into account the distance of the point from the light source and the angle
     * between the light direction and the normal at the point, adjusting the color and intensity
     * accordingly to simulate realistic lighting.
     *
     * @param pixelNorm The normal vector at the point being illuminated.
     * @param pixelPosition The position of the point being illuminated.
     * @param simplCoef A coefficient used to simplify the lighting calculation, affecting the softness of the light.
     * @return The color of the light as it illuminates the point.
     */
    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const override {
        auto toLight = getComponent<TransformMatrix>()->fullPosition() - pixelPosition;
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

    /**
     * @brief Initializes the PointLight component.
     *
     * Ensures that the PointLight has a TransformMatrix component and sets its initial position.
     * This method is called when the PointLight component is added to a scene or needs to be initialized.
     */
    void start() override {
        if (!hasComponent<TransformMatrix>()) {
            // This component requires to work with TransformMatrix component,
            addComponent<TransformMatrix>();
        }

        getComponent<TransformMatrix>()->translate(_initialPos);
    }

    /**
     * @brief Creates a copy of this PointLight instance.
     *
     * Overrides the virtual copy method from the LightSource class to return a new instance of PointLight
     * that is a copy of this instance, using the copy constructor.
     *
     * @return A shared pointer to the new PointLight instance.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<PointLight>(*this);
    }
};

#endif //LIGHTING_POINTLIGHT_H

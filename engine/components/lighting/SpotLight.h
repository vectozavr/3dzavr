#ifndef LIGHTING_SPOTLIGHT_H
#define LIGHTING_SPOTLIGHT_H

#include "LightSource.h"

/**
 * @class SpotLight
 * @brief A spotlight source that emits light in a specific direction with a defined angle.
 *
 * This class represents a spotlight, which is a type of light source that emits light in a cone-shaped beam.
 * The light intensity decreases from the center of the beam to the edges, simulating a realistic spotlight effect.
 * It is defined by its position, direction, inner cone angle, outer cone angle, color, and intensity.
 */
class SpotLight final : public LightSource {
private:
    Vec3D _initialPos; ///< The initial position of the spotlight in the scene.
    Vec3D _dir; ///< The direction in which the spotlight is pointing.
    double _innerConeCos; ///< The cosine of the inner cone angle, defining the core area of the light beam.
    double _outerConeCos; ///< The cosine of the outer cone angle, defining the outer boundary of the light beam.

public:
    /**
     * @brief Constructs a SpotLight object with specified properties.
     *
     * @param position The initial position of the spotlight.
     * @param direction The direction of the spotlight's beam.
     * @param innerConeCos The cosine of the inner cone angle.
     * @param outerConeCos The cosine of the outer cone angle.
     * @param color The color of the light. Defaults to white.
     * @param intensity The intensity of the light. Defaults to 1.0.
     */
    SpotLight(const Vec3D& position, const Vec3D& direction, double innerConeCos = cos(Consts::PI / 4),
              double outerConeCos = cos(Consts::PI / 3), const Color& color = Color::WHITE, double intensity = 1.0):
              LightSource(color, intensity), _dir(direction), _innerConeCos(innerConeCos), _outerConeCos(outerConeCos), _initialPos(position) {};
    SpotLight(const SpotLight& spotLight) = default; ///< Default copy constructor.

    /**
     * @brief Gets the current direction of the spotlight.
     *
     * This method calculates the current direction of the spotlight, taking into account any transformations
     * applied to the light source, such as rotation.
     *
     * @return The current direction of the spotlight as a Vec3D.
     */
    [[nodiscard]] inline Vec3D direction() const { return getComponent<TransformMatrix>()->fullModel()*_dir; };

    [[nodiscard]] double innerConeCos() const { return _innerConeCos; } ///< Gets the cosine of the inner cone angle.
    [[nodiscard]] double outerConeCos() const { return _outerConeCos; } ///< Gets the cosine of the outer cone angle.
    void setInnerConeCos(double innerConeCos) { _innerConeCos = innerConeCos; } ///< Sets the cosine of the inner cone angle.
    void setOuterConeCos(double outerConeCos) { _outerConeCos = outerConeCos; } ///< Sets the cosine of the outer cone angle.

    /**
     * @brief Calculates the color and intensity of the light as it illuminates a given point.
     *
     * This method computes the illumination of a point in the scene by the spotlight. It considers the distance
     * from the light source to the point, the direction of the light, and the angles defining the spotlight's beam.
     * The method applies a linear interpolation based on the simplification coefficient to adjust the precision of
     * the lighting effect. The final color is determined by the light's color, intensity, and the calculated energy
     * based on the spotlight's inner and outer cone angles.
     *
     * @param pixelNorm The normal vector at the point being illuminated.
     * @param pixelPosition The position of the point being illuminated.
     * @param simplCoef A coefficient used to simplify the lighting calculation, affecting the softness of the light.
     * @return The color of the light as it illuminates the point, adjusted for distance and angle.
     */
    [[nodiscard]] Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const override {
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

    /**
     * @brief Initializes the SpotLight component.
     *
     * This method ensures that the SpotLight component is properly initialized with a TransformMatrix component,
     * which is required for its operation. It sets the initial position of the spotlight based on the specified
     * properties. This method is called when the SpotLight component is added to a scene or needs to be initialized.
     */
    void start() override {
        if (!hasComponent<TransformMatrix>()) {
            // This component requires to work with TransformMatrix component,
            addComponent<TransformMatrix>();
        }

        getComponent<TransformMatrix>()->translate(_initialPos);
    }

    /**
     * @brief Creates a copy of this SpotLight instance.
     *
     * This method overrides the virtual copy method from the LightSource class. It creates a new instance
     * of SpotLight that is a copy of this instance, using the copy constructor. This is useful for duplicating
     * spotlights with the same properties and settings.
     *
     * @return A shared pointer to the new SpotLight instance.
     */
    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<SpotLight>(*this);
    }
};

#endif //LIGHTING_SPOTLIGHT_H

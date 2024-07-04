//
// Created by Ivan Ilin on 12/03/2024.
//

#ifndef LIGHTING_LIGHTSOURCE_H
#define LIGHTING_LIGHTSOURCE_H

#include "components/Component.h"

/**
 * @class LightSource
 * @brief A component representing a light source in a scene.
 *
 * LightSource is a component that models the properties of a light source, including its
 * color and intensity. It provides the base functionality for more specific types of light
 * sources and includes methods for setting and getting these properties. It also defines
 * an abstract method `illuminate` that must be implemented by derived classes to calculate
 * the color of a pixel based on the light's properties.
 */
class LightSource : public Component {
protected:
    Color _color = Color::WHITE; ///< The color of the light source.
    double _intensity = 1.0; ///< The intensity of the light source.

public:
    /**
     * @brief Constructs a LightSource with specified color and intensity.
     * @param color The color of the light source.
     * @param intensity The intensity of the light source, clamped to a minimum of 0.0.
     */
    LightSource(const Color& color, double intensity): _color(color), _intensity(std::max(intensity, 0.0)) {}

    /**
     * @brief Copy constructor.
     * @param lightSource A LightSource object to copy from.
     */
    LightSource(const LightSource& lightSource) = default;

    /**
     * @brief Gets the color of the light source.
     * @return The color of the light source.
     */
    [[nodiscard]] inline Color color() const { return _color; };

    /**
     * @brief Gets the intensity of the light source.
     * @return The intensity of the light source.
     */
    [[nodiscard]] inline double intensity() const { return _intensity; };

    /**
     * @brief Calculates the illumination color based on the light source.
     *
     * This is an abstract method that must be implemented by derived classes. It calculates
     * the color of a pixel based on the light source's properties and the pixel's normal and
     * position. The method also takes into account a simplification coefficient for performance
     * optimization.
     *
     * @param pixelNorm The normal vector of the pixel.
     * @param pixelPosition The position of the pixel.
     * @param simplCoef A coefficient for simplifying the computation, ranging from 0 (exact) to 1 (simplified).
     * @return The color of the pixel as influenced by the light source.
     */
    [[nodiscard]] virtual Color illuminate(const Vec3D& pixelNorm, const Vec3D& pixelPosition, double simplCoef) const = 0;

    /**
     * @brief Initializes the LightSource component.
     *
     * This method ensures that a TransformMatrix component is present, adding one if necessary.
     * It is called when the LightSource component is added to a scene or needs to be initialized.
     */
    void start() override {
        if (!hasComponent<TransformMatrix>()) {
            // This component requires to work with TransformMatrix component,
            addComponent<TransformMatrix>();
        }
    }

    /**
     * @brief Sets the intensity of the light source.
     *
     * This method allows for dynamically changing the intensity of the light source. The intensity
     * is a measure of the light's strength or brightness. Higher values result in a brighter light.
     *
     * @param intensity The new intensity value for the light source, clamped to a minimum of 0.0.
     */
    void setIntensity(double intensity) { _intensity = intensity; }

    /**
     * @brief Sets the color of the light source.
     *
     * This method allows for changing the color of the light source. The color can affect the mood
     * and appearance of the scene. It is specified as a Color object, which encapsulates the color's
     * RGB values.
     *
     * @param color The new Color object representing the light source's color.
     */
    void setColor(const Color& color) { _color = color; }
};


#endif //LIGHTING_LIGHTSOURCE_H

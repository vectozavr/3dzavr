#ifndef PROPS_MATERIAL_H
#define PROPS_MATERIAL_H

#include <string>

#include "linalg/Vec3D.h"
#include "Texture.h"

/**
 * @class MaterialTag
 * @brief Represents a tag for categorizing materials.
 *
 * This class encapsulates a tag used to categorize materials in the rendering system. It provides
 * functionality for comparing tags, checking for containment, and retrieving the tag's string representation.
 * Material tags are used to efficiently identify and group materials with similar properties or purposes.
 */
class MaterialTag final {
private:
    const std::string _name; ///< The name of the tag.

public:
    /**
     * @brief Constructs a MaterialTag with an optional name.
     *
     * @param name The name of the tag. Defaults to an empty string if not specified.
     */
    explicit MaterialTag(std::string name = "") : _name(std::move(name)) {}

    /**
     * @brief Returns the name of the tag as a string.
     *
     * @return The name of the tag.
     */
    [[nodiscard]] std::string str() const { return _name; }

    /**
     * @brief Checks for equality between this tag and another tag.
     *
     * @param tag The other MaterialTag to compare with.
     * @return True if the tags are equal, false otherwise.
     */
    bool operator==(const MaterialTag &tag) const { return _name == tag._name; }

    /**
     * @brief Checks for inequality between this tag and another tag.
     *
     * @param tag The other MaterialTag to compare with.
     * @return True if the tags are not equal, false otherwise.
     */
    bool operator!=(const MaterialTag &tag) const { return _name != tag._name; }

    /**
     * @brief Compares this tag with another tag to determine order.
     *
     * @param tag The other MaterialTag to compare with.
     * @return True if this tag is less than the other tag, false otherwise.
     */
    bool operator<(const MaterialTag &tag) const { return _name < tag._name; }

    /**
     * @brief Checks if this tag contains another tag.
     *
     * This method can be used to implement hierarchical or nested tagging systems where one tag
     * might encompass another. The default implementation does not perform any actual containment
     * checks and should be overridden if such functionality is desired.
     *
     * @param tag The MaterialTag to check for containment within this tag.
     * @return True if this tag contains the other tag, false otherwise.
     */
    [[nodiscard]] bool contains(const MaterialTag& tag) const;
};

/**
 * @class Material
 * @brief Represents a material in the rendering system.
 *
 * This class encapsulates the properties of a material used in rendering objects. It includes
 * references to texture maps, color properties (ambient, diffuse, specular), illumination model,
 * transparency, and a tag for categorization. The class provides methods to access and modify these properties.
 */
class Material {
private:
    const MaterialTag _tag; ///< Tag used for categorizing materials.
    std::shared_ptr<Texture> _texture = nullptr; ///< Base texture map.
    // TODO: add other maps like normal map, shadow map and eth..
    // std::shared_ptr<Texture> _normalMap = nullptr;
    // std::shared_ptr<Texture> _shadowMap = nullptr;
    // std::shared_ptr<Texture> _diffusionMap = nullptr;
    // ...

    Color _ambient; ///< Ambient color component.
    Color _diffuse; ///< Diffuse color component.
    Color _specular; ///< Specular color component.
    uint16_t _illum; ///< Illumination model identifier.
    double _d = 1.0; ///< Transparency value.

    bool _isTransparent = false; ///< Indicates if the material is transparent.

    /**
     * @brief Checks and updates the transparency status of the material.
     */
    void checkTransparent();
public:
    /**
     * @brief Constructs a Material with specified properties.
     *
     * @param tag MaterialTag for categorizing the material.
     * @param texture Shared pointer to the base Texture.
     * @param ambient Ambient color component.
     * @param diffuse Diffuse color component.
     * @param specular Specular color component.
     * @param illum Illumination model identifier.
     * @param d Transparency value.
     */
    Material(const MaterialTag& tag,
             std::shared_ptr<Texture> texture,
             const Color& ambient = Color::WHITE,
             const Color& diffuse = Color::WHITE,
             const Color& specular = Color::WHITE,
             uint8_t illum = 1,
             double d = 1);

    /**
     * @brief Gets the ambient color component of the material.
     *
     * This method returns the ambient color of the material, which represents the color of the material
     * under ambient lighting. Ambient lighting is the general illumination in the scene that affects all objects equally.
     *
     * @return The ambient color component of the material.
     */
    [[nodiscard]] Color ambient() const { return _ambient; }

    /**
     * @brief Gets the diffuse color component of the material.
     *
     * This method returns the diffuse color of the material, which represents the color of the material
     * under diffuse lighting. Diffuse lighting simulates the scattering of light in many directions after hitting a surface.
     *
     * @return The diffuse color component of the material.
     */
    [[nodiscard]] Color diffuse() const { return _diffuse; }

    /**
     * @brief Gets the specular color component of the material.
     *
     * This method returns the specular color of the material, which represents the color of the material
     * under specular lighting. Specular lighting simulates the bright spot of light that appears on shiny objects.
     *
     * @return The specular color component of the material.
     */
    [[nodiscard]] Color specular() const { return _specular; }

    /**
     * @brief Gets the illumination model identifier of the material.
     *
     * This method returns the identifier for the illumination model used by the material. The illumination model
     * determines how the material interacts with light in terms of reflection, refraction, and shadow casting.
     *
     * @return The illumination model identifier.
     */
    [[nodiscard]] uint8_t illum() const { return _illum; }

    /**
     * @brief Gets the transparency value of the material.
     *
     * This method returns the transparency value of the material, which determines how transparent the material is.
     * A value of 1.0 means fully opaque, while values closer to 0.0 indicate higher transparency.
     *
     * @return The transparency value of the material.
     */
    [[nodiscard]] double d() const { return _d; }

    /**
     * @brief Sets the ambient color component of the material.
     *
     * This method allows setting the ambient color of the material, affecting how the material appears under ambient lighting.
     *
     * @param color The new ambient color for the material.
     */
    void setAmbient(const Color& color);

    /**
     * @brief Sets the transparency value of the material.
     *
     * This method allows setting the transparency value of the material, affecting how transparent the material appears.
     *
     * @param d The new transparency value for the material.
     */
    void setTransparency(double d);

    /**
     * @brief Sets the base texture of the material.
     *
     * This method allows setting the base texture of the material, which is used to texture the material surface.
     *
     * @param texture Shared pointer to the new base texture for the material.
     */
    void setTexture(std::shared_ptr<Texture> texture);

    /**
     * @brief Retrieves the base texture of the material.
     *
     * This method returns a shared pointer to the base texture of the material. The base texture is used
     * to texture the material surface and is a fundamental component of the material's visual appearance.
     *
     * @return A shared pointer to the Texture object representing the base texture of the material.
     */
    [[nodiscard]] std::shared_ptr<Texture> texture() const {return _texture; }

    /**
     * @brief Retrieves the tag associated with the material.
     *
     * This method returns the MaterialTag of the material, which is used for categorizing and identifying
     * materials within the rendering system. The tag facilitates the efficient management and retrieval of materials.
     *
     * @return The MaterialTag associated with the material.
     */
    [[nodiscard]] MaterialTag tag() const { return _tag; }

    /**
     * @brief Checks if the material is transparent.
     *
     * This method returns a boolean indicating whether the material is transparent. Transparency is determined
     * by the transparency value (_d) of the material. Materials with transparency can affect rendering techniques,
     * such as sorting and blending.
     *
     * @return True if the material is transparent, false otherwise.
     */
    [[nodiscard]] bool isTransparent() const {return _isTransparent; }
};


#endif //PROPS_MATERIAL_H

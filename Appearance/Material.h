/*
 * Material.h
 *
 *  Created on:
 Shape15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/Node.h>

#include <OgrePass.h>
#include <OgreBlendMode.h>

#include <Appearance/ImageTexture.h>

namespace X3D {
/**
 * Configures the Color paramters of a Ogre rendering pass
 */
class Material: public Node {
	float _ambientIntensity = 0.2;
    Ogre::ColourValue _diffuseColor;
    Ogre::ColourValue _emissiveColor = Ogre::ColourValue::Black;
    Ogre::ColourValue _specularColor = Ogre::ColourValue::Black;
    float _alpha = 1;
    float _shininess = 0.2;

    bool _isLit;

    Ogre::Pass* _pass = nullptr;
public:
    static Material DEFAULT; // default unlit material

    enum DefaultMaterial {
        MAT_DEFAULT_LIT,
        MAT_DEFAULT_UNLIT
    };

    explicit Material(DefaultMaterial init = MAT_DEFAULT_LIT);

    /**
     * Specifies the ambient Intensity of the Material
     * @param ambientIntensity
     */
    void ambientIntensity(const float& ambientIntensity);

    /**
     * Specifies the diffuse Color of the Material
     * @param diffuseColor
     */
    void diffuseColor(const Ogre::ColourValue& diffuseColor);

    /**
	 * Specifies the emissive Color of the Material
	 * @param emissiveColor
	 */
	void emissiveColor(const Ogre::ColourValue& emissiveColor);

	/**
	 * Specifies the specular Color of the Material
	 * @param specularColor
	 */
	void specularColor(const Ogre::ColourValue& specularColor);

	/**
	 * Speicifiy Shininess
	 * Lower shininess values produce soft glows, while higher values result in
	 * sharper, smaller highlights.
	 * @param shininess
	 */
	void shininess(const float& shininess);

    /**
     * Specifies the transparency of the Material
     * 	Transparency == 1.0: Object is fully transparent
     * 	Transparency == 0.0: Object is solid
     * 	Transparency = 1-alpha
     * @param alpha Transparency, Value between 0 and 1
     */
    void transparency(const float& alpha);

    /**
     * Returns the diffuse Color of the Material
     * @return Diffuse Color
     */
    const Ogre::ColourValue& diffuseColor() const;

    /**
	 * Returns the emissive Color of the Material
	 * @return Emissive Color
	 */
	const Ogre::ColourValue& emissiveColor() const {
		return _emissiveColor;
	}

	/**
	 * Returns the specular Color of the Material
	 * @return Specular Color
	 */
	const Ogre::ColourValue& specularColor() const {
		return _specularColor;
	}

	/**
	 * Returns the ambient Intensity of the Material
	 * @return ambient Intensity
	 */
	const float& ambientIntensity() const {
		return _ambientIntensity;
	}

	/**
	 * Returns the shininess of the Material
	 * @return shininess
	 */
	const float& shininess() const {
		return _shininess;
	}

    /**
     * apply configuration to pass
     * @param pass
     */
    void apply(Ogre::Pass* pass);

};

}

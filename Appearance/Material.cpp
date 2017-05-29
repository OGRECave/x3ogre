/*
 * Material.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include "Material.h"

#include <OgreRoot.h>
#include <OgreMaterial.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreTechnique.h>

using namespace X3D;

Material Material::DEFAULT(MAT_DEFAULT_UNLIT);

/// Constructor
Material::Material(DefaultMaterial init){
    switch(init) {
    case MAT_DEFAULT_LIT:
        _diffuseColor = Ogre::ColourValue(0.8,0.8,0.8);
        _isLit = true;
        break;
    case MAT_DEFAULT_UNLIT:
        // DefaultMaterial means that no Material is specified.
        // In that case lighting has no effect and the Object is just white.
        _diffuseColor = Ogre::ColourValue::White;
        _isLit = false;
        break;
    }
}

void Material::ambientIntensity(const float& ambientIntensity) {
	_ambientIntensity = ambientIntensity;

	if(not _pass) {
	    return;
	}

    // Set Ambient Color to standard ambient intensity (See InstantReality Doc)
    _pass->setAmbient(_ambientIntensity * _diffuseColor);
}

void Material::diffuseColor(const Ogre::ColourValue& diffuseColor) {
	_diffuseColor = diffuseColor;
    _diffuseColor.a = _alpha;

    if(not _pass) {
        return;
    }

    _pass->setDiffuse(_diffuseColor);
    // Set Ambient Color to standard ambient intensity (See InstantReality Doc)
    _pass->setAmbient(_ambientIntensity * _diffuseColor);
}

void Material::emissiveColor(const Ogre::ColourValue& emissiveColor) {
	_emissiveColor = emissiveColor;

    if(not _pass) {
        return;
    }

    _pass->setSelfIllumination(_emissiveColor);
}

void Material::specularColor(const Ogre::ColourValue& specularColor) {
	_specularColor = specularColor;

    if(not _pass) {
        return;
    }

    _pass->setSpecular(_specularColor);
}

void Material::shininess(const float& shininess) {
	_shininess = shininess;

    if(not _pass) {
        return;
    }

    _pass->setShininess(_shininess*128.0);
}

const Ogre::ColourValue& Material::diffuseColor() const {
	return _diffuseColor;
}

void Material::apply(Ogre::Pass* pass){
    _pass = pass;

    // in first call all colors are at default values

    transparency(1.0 - _alpha); // also sets diffuse
    ambientIntensity(_ambientIntensity);
    shininess(_shininess);
    specularColor(_specularColor);
    emissiveColor(_emissiveColor);

    _pass->setLightingEnabled(_isLit);
}

void Material::transparency(const float& val) {
    _alpha = 1.0 - val;
    _diffuseColor.a = _alpha;

    if(not _pass) {
        return;
    }

    _pass->setDiffuse(_diffuseColor);

    // If an alpha-value < 1 is set, disable depthbuffer for this material to
    //      Enable blending Effects
    if (_alpha < 1.0) {
        _pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        _pass->setDepthWriteEnabled(false);
    } else {
        _pass->setSceneBlending(Ogre::SBT_REPLACE);
        _pass->setDepthWriteEnabled(true);
    }
}

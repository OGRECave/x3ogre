/*
 * DirectionalLight.cpp
 *
 *  Created on: 01.04.2014
 *      Author: baudenri
 */

#include <World/DirectionalLight.h>
#include <core/World.h>
#include <OgreSceneNode.h>

namespace X3D {

// Setter for variables
void DirectionalLight::direction(const Ogre::Vector3& direction) {
	_direction = direction;
}
void DirectionalLight::color(const Ogre::ColourValue& color) {
	_color = color;
}
void DirectionalLight::ambientIntensity(const float& ambientIntensity) {
	_ambientIntensity = ambientIntensity;
}
void DirectionalLight::intensity(const float& intensity) {
	_intensity = intensity;
}
void DirectionalLight::global(const bool& global) {
	_global = global;
}
void DirectionalLight::on(const bool& on) {
	_on = on;

	if(not _init) {
	    return;
	}

	if (_on) {
		_light->setDiffuseColour(_color*_intensity);
		_light->setSpecularColour(_color*_intensity);
	} else {
		_light->setDiffuseColour(Ogre::ColourValue::Black);
		_light->setSpecularColour(Ogre::ColourValue::Black);
	}
}

void DirectionalLight::attachTo(Ogre::SceneNode* node) {
	if (_light && _light->isAttached()) {
		_light->detachFromParent();
	}
    _parent = node;
    if (_parent && _light) {
    	_parent->attachObject(_light);
		_light->setVisible(true);
    }
}

void DirectionalLight::detachFromParent() {
	if (_parent) _parent->detachObject(_light);
	if (_light) _light->setVisible(false);
	_parent = nullptr;
}

/**
 * Initializes the Light-Source and attaches it to the scene
 * @param world
 */
void DirectionalLight::initialise(World& world) {
	if (_init) {
		return;
	}
    _init = true;

	_light = world.sceneManager()->createLight();
	_light->setType(Ogre::Light::LT_DIRECTIONAL);

	if(_parent) {
	    _parent->attachObject(_light);
		_parent->setDirection(_direction);
	}

	// Light is always present but maybe is off (_headLight==false)
	on(_on);
	world.sceneManager()->setAmbientLight(_ambientIntensity*_color);
}

}

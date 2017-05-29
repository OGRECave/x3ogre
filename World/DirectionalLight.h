/*
 * DirectionalLight.h
 *
 *  Created on: 01.04.2014
 *      Author: baudenri
 */

#pragma once
#include <core/Node.h>

#include <OgrePrerequisites.h>
#include <OgreColourValue.h>
#include <OgreVector3.h>

namespace X3D {

/**
 * The DirectionalLight node defines a directional light source that illuminates
 * along rays parallel to a given 3-dimensional vector.
 */

class DirectionalLight: public Node {
	float _ambientIntensity = 0;
	Ogre::ColourValue _color = Ogre::ColourValue::White;
	Ogre::Vector3 _direction = Ogre::Vector3(0, 0, -1);
	bool _global = false;
	float _intensity = 1;
	bool _on = true;

	Ogre::Light* _light = nullptr;
	Ogre::SceneNode* _parent = nullptr;
public:

	// Setter for variables
	void ambientIntensity(const float& ambientIntensity);
	void intensity(const float& intensity);
	/**
	 * Not used in current implementation
	 */
	void global(const bool& global);
	void on(const bool& on);
	void direction(const Ogre::Vector3& direction);
	void color(const Ogre::ColourValue& color);

	void attachTo(Ogre::SceneNode* node);

	/**
	 * Initializes the Light-Source and attaches it to the scene
	 * @param world
	 */
	void initialise(World& world) override;

	void detachFromParent();
};

}

/*
 * Cone.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#pragma once
#include <Geometry/Geometry.h>

namespace X3D {

/**
 * Simple Geometrical Object: Cone
 */
class Cone: public Geometry {
	float _radius = 1;
	float _height = 2;
public:
	/**
	 * Specifies the Radius of the Cone
	 * @param radius
	 */
	void radius(const float& radius);
	/**
	 * Specifies the Height of the Cone
	 * @param height
	 */
	void height(const float& height);

	/**
	 * Creates the Mesh of the Cone
	 */
	void loadResource(Ogre::Resource* resource);
};

} /* namespace X3D */

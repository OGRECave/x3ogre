/*
 * Sphere.h
 *
 *  Created on: 21.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <Geometry/Geometry.h>

namespace X3D
{

/**
 * Simple Geometrical Object: Sphere
 */
class Sphere: public Geometry{
	float _radius = 1;

public:
	/**
	 * Specifies the Radius of the sphere
	 * @param radius
	 */
	void radius(const float& radius);

	/**
	 * Creates the Mesh of the Sphere
	 */
	void loadResource(Ogre::Resource* resource);
};

}

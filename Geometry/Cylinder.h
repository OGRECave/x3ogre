/*
 * Cylinder.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#pragma once
#include <Geometry/Geometry.h>

namespace X3D {

/**
 * Simple Geometrical Object: Cylinder
 */
class Cylinder: public Geometry {
    float _radius = 1;
    float _height = 2;

public:
    /**
     * Specifies the radius of the Cylinder
     * @param radius
     */
    void radius(const float& radius);
    /**
     * Specifies the height of the Cylinder
     * @param height
     */
    void height(const float& height);

	/**
	 * Creates the Mesh of the Cylinder
	 */
    void loadResource(Ogre::Resource* resource);
};

} /* namespace X3D */

/*
 * Rectangle2D.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#pragma once
#include <Geometry/Geometry.h>

namespace X3D
{

/**
 * Simple Geometrical Object: Rectangle2D
 */
class Rectangle2D: public Geometry{
	Ogre::Vector2 _size = Ogre::Vector2(2, 2);

public:
	/**
	 * Specifies the Dimension of the Rectangle
	 * @param size length and height
	 */
	void size(const Ogre::Vector2& size);

	/**
	 * Creates the Mesh of the Rectangle
	 */
	void loadResource(Ogre::Resource* resource);
};

}

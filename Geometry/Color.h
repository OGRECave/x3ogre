/*
 * Color.h
 *
 *  Created on: 02.04.2014
 *      Author: baudenri
 */
/*
 * Appearance.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/Node.h>
#include <vector>
#include <OgreVector3.h>

namespace X3D {

/**
 * X3D::Color represents an vector of Ogre::Vector3 which represents ColorValues
 * This is associated to a Geometrical Object
 */
class Color: public Node {

	std::vector<Ogre::Vector3> _color;

public:

	Color();

	void color(const std::vector<Ogre::Vector3>& color);

	const std::vector<Ogre::Vector3>& getColor();


};

}

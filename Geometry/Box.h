/*
 * Box.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <Geometry/Geometry.h>

namespace X3D {

/**
 * Simple Geometrical Object: Box
 */
class Box: public Geometry {
    Ogre::Vector3 _size = Ogre::Vector3(2, 2, 2);

public:
    /**
     * Specifies the Size of the Box
     * @param size Height*Width*Depth
     */
    void size(const Ogre::Vector3& size);

    /**
     * Creates the Mesh of the Box
     */
	void loadResource(Ogre::Resource* resource);
};

}

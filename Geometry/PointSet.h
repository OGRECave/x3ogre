/*
 * PointSet.h
 *
 *  Created on: 20.08.2015
 *      Author: baudenri
 */

#pragma once
#include "IndexedPrimitiveSet.h"

namespace X3D {

/**
 * Set of Points
 */
class PointSet: public IndexedPrimitiveSet {
public:

	 /**
	 * Creates the Mesh of the PointSet
	 */
    void loadResource(Ogre::Resource* resource);
};

}

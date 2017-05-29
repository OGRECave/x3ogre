/*
 * IndexedLineSet.h
 *
 *  Created on: 05.12.2013
 *      Author: baudenri_local
 */

#pragma once
#include "IndexedPrimitiveSet.h"

namespace X3D {

/**
 * Set of Lines
 */
class IndexedLineSet: public IndexedPrimitiveSet {
	 std::vector<int> _index;
public:

    /**
	 * Specifices the Vector of the Line Indexes
	 * 	At least 2 indexes seperated by a seperator (e.g. -1)
	 * @param index Vector of Line Indexes.
	 */
	void coordIndex(const std::vector<int>& index);

	 /**
	 * Creates the Mesh of the LineSet
	 * @param sceneManager
	 * @param material Name of the used material
	 */
    void loadResource(Ogre::Resource* resource);
};

}

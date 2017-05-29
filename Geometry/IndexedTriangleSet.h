/*
 * IndexedTriangleSet.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#pragma once

#include "IndexedPrimitiveSet.h"
#include <Geometry/Color.h>

namespace X3D {

/**
 * Set of Triangles which fit together to a more complex structure
 */
class IndexedTriangleSet: public IndexedPrimitiveSet {
    bool _normalPerVertex = true;
    std::vector<uint32_t> _index;

public:
	/**
	 * Specifices the Vector of the Triangle Indexes
	 * 	3 following Indexes specify one triangle
	 * @param index Vector of Triangle Indexes. It has to be devisible by 3.
	 */
    void index(const std::vector<int>& index);


    void normalPerVertex(const bool& val);

    /**
     * always ignored and assumed to be TRUE (per X3D standard)
     * @param val
     */
    void colorPerVertex(const bool& val) {
    }

    /**
	 * Creates the Mesh of the TriangleSet
	 */
    void loadResource(Ogre::Resource* resource);
};

}

/*
 * IndexedFaceSet.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#pragma once

#include "IndexedPrimitiveSet.h"

namespace X3D {

class IndexedFaceSet: public IndexedPrimitiveSet {
    float _creaseAngle = M_PI;
    std::vector<int> _index;
    std::vector<int> _colorIndex;
    std::vector<int> _texCoordIndex;

    bool _colorPerVertex;
    bool _ccw = true;
public:
    void creaseAngle(const float& val);

	/**
	 * Specifices the Vector of the Face Indexes
	 * 	At least 3 indexes seperated by a seperator (e.g. -1)
	 * @param index Vector of Face Indexes.
	 */
    void coordIndex(const std::vector<int>& index);

	/**
	 * Specifies the Vector of the Colors
	 * 	If colorPerVertex = FALSE:
	 * 		colorIndex[i] specifies the Color of the i-th Face
	 * 	If colorPerVertex = TRUE:
	 * 		colorIndex[i] specifies the Color of the i-th Vertex
	 * @param index Vector of Colors.
	 */
    void colorIndex(const std::vector<int>& colorIndex);

    /**
     * Specifies how Colors are applied to the Mesh:
     * 	If colorPerVertex = FALSE:
	 * 		colorIndex[i] specifies the Color of the i-th Face
	 * 	If colorPerVertex = TRUE:
	 * 		colorIndex[i] specifies the Color of the i-th Vertex
     */
    void colorPerVertex(const bool& colorPerVertex);

    bool ccw() const;

    void ccw(const bool& ccw);

    /**
   	 * Creates the Mesh of the FaceSet
   	 */
    void loadResource(Ogre::Resource* resource);
};

}

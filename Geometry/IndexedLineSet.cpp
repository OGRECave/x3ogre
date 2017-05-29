/*
 * IndexedLineSet.cpp
 *
 *  Created on: 05.12.2013
 *      Author: baudenri_local
 */

#include <Geometry/IndexedLineSet.h>

#include <GeoShape/Utils.h>
#include <core/OgreMeshAdapter.h>

#include <Geometry/Coordinate.h>
#include <Geometry/Normal.h>
#include <Geometry/TextureCoordinate.h>
#include <Geometry/Color.h>

using namespace X3D;

void IndexedLineSet::coordIndex(const std::vector<int>& index) {
	_index = index;
}

void IndexedLineSet::loadResource(Ogre::Resource* resource) {

	std::vector<Ogre::Vector3> points = _coordinate->point();
	std::vector<Ogre::Vector3> normalsGen;
	std::vector<Ogre::Vector2> texCoord;

	if (_textureCoordinate) {
		texCoord = _textureCoordinate->point();
	}

	const std::vector<Ogre::Vector3>& normals = _normal ? _normal->vector() : normalsGen;

	// Convert Line Strip to Line List
	std::vector<uint32_t> index;
	GeoShape::LineStripToList(_index, index);

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_LINE_LIST, index, points, normals, texCoord);
}

/*
 * Rectangle2D.cpp
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#include <Geometry/Rectangle2D.h>
#include <core/OgreMeshAdapter.h>
#include <GeoShape/Primitive.h>

using namespace X3D;

void Rectangle2D::size(const Ogre::Vector2& size){
	_size = size;
}

void Rectangle2D::loadResource(Ogre::Resource* resource){
	std::vector<GeoShape::Vector2> vtx;
    std::vector<GeoShape::Vector3> nrm;
    std::vector<GeoShape::Vector2> tex;

    GeoShape::Rectangle(vtx, nrm, tex, _size/2);

	std::vector<uint16_t> index;
	std::vector<GeoShape::Vector3> vertex;
	std::vector<GeoShape::Vector3> normal;
	std::vector<GeoShape::Vector2> texture;

	for(size_t i = 0; i < vtx.size(); i++) {
		index.push_back(i);
		vertex.push_back(GeoShape::Vector3(vtx[i][0], vtx[i][1], 0));
		normal.push_back(nrm[i]);
		texture.push_back(tex[i]);
	}
	for(size_t i = vtx.size(); i > 0; i--) {
		index.push_back(2*vtx.size()-i);
		vertex.push_back(GeoShape::Vector3(vtx[i-1][0], vtx[i-1][1], 0));
		normal.push_back(-nrm[i-1]);
		texture.push_back(tex[i-1]);
	}

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_TRIANGLE_FAN, index, vertex, normal, texture);

}


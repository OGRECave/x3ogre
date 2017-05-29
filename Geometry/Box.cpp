/*
 * Box.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <Geometry/Box.h>
#include <core/OgreMeshAdapter.h>
#include <GeoShape/Primitive.h>


using namespace X3D;

void Box::size(const Ogre::Vector3& size){
	_size = size;
}

void Box::loadResource(Ogre::Resource* resource){
    std::vector<uint16_t> idx;
    std::vector<GeoShape::Vector3> vtx;
    std::vector<GeoShape::Vector3> nrm;
    std::vector<GeoShape::Vector2> tex;

    GeoShape::Box(idx, vtx, nrm, tex, _size/2);

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_TRIANGLE_LIST, idx, vtx, nrm, tex);
}

/*
 * PointSet.cpp
 *
 *  Created on: 20.08.2015
 *      Author: baudenri
 */

#include <Geometry/PointSet.h>

#include <core/OgreMeshAdapter.h>

#include <Geometry/Coordinate.h>

#include <OgreSubMesh.h>

using namespace X3D;

void PointSet::loadResource(Ogre::Resource* resource) {
    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_POINT_LIST,
    		std::vector<uint32_t>(), _coordinate->point(),
    		std::vector<Ogre::Vector3>(), std::vector<Ogre::Vector2>() );

}

/*
 * Sphere.cpp
 *
 *  Created on: 21.11.2013
 *      Author: baudenri_local
 */

#include <Geometry/Sphere.h>

#include <core/OgreMeshAdapter.h>
#include <GeoShape/Primitive.h>

namespace X3D {

void Sphere::radius(const float& radius) {
	_radius = radius;
}

void Sphere::loadResource(Ogre::Resource* resource) {
    std::vector<uint16_t> idx;
    std::vector<GeoShape::Vector3> vtx;
    std::vector<GeoShape::Vector3> nrm;
    std::vector<GeoShape::Vector2> tex;

    GeoShape::Sphere(idx, vtx, nrm, tex, Ogre::Vector3(_radius));

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_TRIANGLE_LIST, idx, vtx, nrm, tex);
}

}

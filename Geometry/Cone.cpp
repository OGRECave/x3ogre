/*
 * Cone.cpp
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */

#include <Geometry/Cone.h>

#include <core/OgreMeshAdapter.h>
#include <GeoShape/Primitive.h>

using namespace X3D;

void Cone::radius(const float& radius) {
    _radius = radius;
}

void Cone::height(const float& height) {
    _height = height;
}

void Cone::loadResource(Ogre::Resource* resource) {
    Ogre::Vector3 size(_radius, _height / 2, _radius);

    std::vector<uint16_t> idx;
    std::vector<GeoShape::Vector3> vtx;
    std::vector<GeoShape::Vector3> nrm;
    std::vector<GeoShape::Vector2> tex;

    GeoShape::Cone(idx, vtx, nrm, tex, size);

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_TRIANGLE_LIST, idx, vtx, nrm, tex);

}

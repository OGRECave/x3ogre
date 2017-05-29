/*
 * IndexedTriangleSet.cpp
 *
 *  Created on: 05.12.2013
 *      Author: baudenri_local
 */

#include <Geometry/IndexedTriangleSet.h>

#include <OgreMeshManager.h>

#include <GeoShape/Utils.h>
#include <core/OgreMeshAdapter.h>

#include <Geometry/Coordinate.h>
#include <Geometry/Normal.h>
#include <Geometry/TextureCoordinate.h>
#include <Geometry/Color.h>

namespace X3D {

void IndexedTriangleSet::index(const std::vector<int>& index) {
    _index.insert(_index.end(), index.begin(), index.end());
}

void IndexedTriangleSet::normalPerVertex(const bool& val) {
    _normalPerVertex = val;
}

void IndexedTriangleSet::loadResource(Ogre::Resource* resource) {
    std::vector<Ogre::Vector3> points = _coordinate->point();
    std::vector<Ogre::Vector3> normalsGen;
    std::vector<Ogre::Vector2> texCoord;
    std::vector<Ogre::Vector3> colorsLocal;

    if(_textureCoordinate) {
        texCoord = _textureCoordinate->point();
    }

    if (not _normal) {
        if (_normalPerVertex) {
            GeoShape::GenerateNormals(_index, points, normalsGen);
        } else {
            std::vector<Ogre::Vector3> tmp;
            GeoShape::DuplicateByIndex(_index, points, tmp);
            points.swap(tmp);
            GeoShape::GenerateNormals(points, normalsGen);
            _index.clear();
        }
    }

    const auto& normals = _normal ? _normal->vector() : normalsGen;
    const auto& colors = _color ? _color->getColor() : colorsLocal;

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_TRIANGLE_LIST, _index, points, normals, texCoord, colors);

    auto ci = _coordinate->interpolator();
    if(ci) {
		ci->addToMesh(static_cast<Ogre::Mesh*>(resource));
    }
}
}

/*
 * IndexedFaceSet.cpp
 *
 *  Created on: 05.12.2013
 *      Author: baudenri_local
 */

#include <Geometry/IndexedFaceSet.h>
#include <GeoShape/Utils.h>

#include <core/OgreMeshAdapter.h>

#include <Geometry/Coordinate.h>
#include <Geometry/Normal.h>
#include <Geometry/TextureCoordinate.h>
#include <Geometry/Color.h>

#include <OgreMeshManager.h>

using namespace X3D;

void IndexedFaceSet::coordIndex(const std::vector<int>& index) {
    _index = index;
}

void IndexedFaceSet::creaseAngle(const float& val) {
    _creaseAngle = val;
}

void IndexedFaceSet::colorIndex(const std::vector<int>& colorIndex) {
	_colorIndex = colorIndex;
}

void IndexedFaceSet::colorPerVertex(const bool& colorPerVertex) {
	_colorPerVertex = colorPerVertex;
}

bool IndexedFaceSet::ccw() const {
    return _ccw;
}

void IndexedFaceSet::ccw(const bool& ccw) {
    _ccw = ccw;
}

void IndexedFaceSet::loadResource(Ogre::Resource* resource) {
    std::vector<uint32_t> indexTri;
    GeoShape::Triangulate(_index, indexTri);

    std::vector<Ogre::Vector3> points = _coordinate->point();
    std::vector<Ogre::Vector3> normalsGen;
    std::vector<Ogre::Vector2> texCoord;
    std::vector<Ogre::Vector3> colorsLocal;

    if(_textureCoordinate) {
        texCoord = _textureCoordinate->point();
    }

    if (not _normal) {
        if (_creaseAngle > std::numeric_limits<float>::epsilon()) {
            GeoShape::GenerateNormals(indexTri, points, normalsGen);
        } else {
            // crease angle is zero
            std::vector<Ogre::Vector3> tmp;
            GeoShape::DuplicateByIndex(indexTri, points, tmp);
            points.swap(tmp);
            GeoShape::GenerateNormals(points, normalsGen);
            indexTri.clear();

            if(not texCoord.empty()) {
                std::vector<Ogre::Vector2> tmp;
                GeoShape::DuplicateByIndex(indexTri, texCoord, tmp);
                texCoord.swap(tmp);
            }
        }
    }

    const auto& normals = _normal ? _normal->vector() : normalsGen;
    const auto& colors = _color ? _color->getColor() : colorsLocal;

    OgreMeshAdapter(static_cast<Ogre::Mesh*>(resource), Ogre::RenderOperation::OT_TRIANGLE_LIST, indexTri, points, normals, texCoord, colors);

    auto ci = _coordinate->interpolator();
    if(ci) {
    	ci->addToMesh(static_cast<Ogre::Mesh*>(resource));
    }
}

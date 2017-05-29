/*
 * TextureCoordinate2D.cpp
 *
 *  Created on: 24.01.2014
 *      Author: baudenri
 */

#include <Geometry/TextureCoordinate.h>

namespace X3D {

void TextureCoordinate::point(const std::vector<Ogre::Vector2>& point) {
    _point = point;
}

const std::vector<Ogre::Vector2>& TextureCoordinate::point() const {
    return _point;
}

}

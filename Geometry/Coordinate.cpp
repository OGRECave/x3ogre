/*
 * Coordinates.cpp
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */


#include <Geometry/Coordinate.h>

namespace X3D {

void Coordinate::point(const std::vector<Ogre::Vector3>& val){
	_point = val;
}

std::vector<Ogre::Vector3>& Coordinate::point() {
	return _point;
}

void Coordinate::set_point(const std::shared_ptr<CoordinateInterpolator>& ci) {
    _interpolator = ci;
}

const std::shared_ptr<CoordinateInterpolator>& Coordinate::interpolator() const {
    return _interpolator;
}

}



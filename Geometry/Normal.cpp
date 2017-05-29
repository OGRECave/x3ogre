/*
 * Normal.cpp
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */


#include <Geometry/Normal.h>

using namespace X3D;

void Normal::vector(const std::vector<Ogre::Vector3>& val){
    _vector = val;
}

std::vector<Ogre::Vector3>& Normal::vector() {
	return _vector;
}

/*
 * Normal.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */
#pragma once
#include <Geometry/Geometry.h>
#include <vector>

namespace X3D {

class Normal: public Node {
    std::vector<Ogre::Vector3> _vector;
public:
    void vector(const std::vector<Ogre::Vector3>& val);
    std::vector<Ogre::Vector3>& vector();
};

}

/*
 * Color.cpp
 *
 *  Created on: 02.04.2014
 *      Author: baudenri
 */

#include <Geometry/Color.h>

using namespace X3D;

Color::Color() {

}

void Color::color(const std::vector<Ogre::Vector3>& color) {
	_color = color;
}

const std::vector<Ogre::Vector3>& Color::getColor() {
	return _color;
}

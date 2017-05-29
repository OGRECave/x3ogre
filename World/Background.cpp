/*
 * Background.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <World/Background.h>

namespace X3D {

void Background::skyColor(const Ogre::ColourValue& skyColour){
	_skyColor = skyColour;

	if(not _viewport) {
	    return;
	}

	_viewport->setBackgroundColour(_skyColor);
}

const Ogre::ColourValue& Background::skyColor() const {
	return _skyColor;
}

void Background::apply(Ogre::Viewport* viewport) {
    _viewport = viewport;
    _viewport->setBackgroundColour(_skyColor);
}

void Background::onBound(Scene& scene) {
	if (scene.getViewport()) {
		apply(scene.getViewport());
	}
}

void Background::onUnbound(Scene& scene) {
	_viewport = nullptr;
}

}
